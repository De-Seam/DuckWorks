#include "Precomp.h"
#include "Engine/Collision/CollisionWorld.h"

// Engine includes
#include "Engine/Collision/CollisionHelperFunctions.h"
#include "Engine/Collision/CollisionStructs.h"
#include "Engine/Debug/DebugUIFunctions.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Threads/ThreadManager.h"

RTTI_CLASS_DEFINITION(CollisionWorld, StandardAllocator)

RTTI_EMPTY_SERIALIZE_DEFINITION(CollisionWorld)

CollisionWorld::CollisionWorld(const ConstructParameters& inConstructParameters)
	: Base(inConstructParameters)
{
	BVH::InitParams params;
	params.mCollisionWorld = this;
	mBVH.Init(params);
}

void CollisionWorld::BeginPlay()
{
	mBVH.Generate();
}

void CollisionWorld::Draw()
{
	DrawBVH();
	DrawCollision();
}

void CollisionWorld::DrawCollision()
{
	LoopCollisionObjects([](const CollisionObject& inObject)
	{
		Renderer::DrawFilledRectangleParams params;
		params.mTransform = inObject.GetTransform();
		params.mColor = {0.f, 1.f, 0.f, 0.5f};
		params.mLayer = Renderer::EDrawLayer::Effects;
		gRenderer.DrawFilledRectangle(params);
	});
}

void CollisionWorld::DrawBVH()
{
	mBVH.Draw();
}

CollisionObjectHandle CollisionWorld::CreateCollisionObject(const CollisionObject::ConstructParameters& inConstructParameters)
{
	PROFILE_SCOPE(CollisionWorld::CreateCollisionObject)

	CollisionObjectHandle handle = FindOrCreateCollisionObjectIndex(inConstructParameters);
	mBVH.AddObject(handle);
	if (!mBVH.IsGenerated())
		mBVH.Generate();
	return handle;
}

void CollisionWorld::DestroyCollisionObject(const CollisionObjectHandle& inObjectHandle)
{
	PROFILE_SCOPE(CollisionWorld::DestroyCollisionObject)
	gAssert(gIsMainThread());

	gAssert(mCollisionObjects[inObjectHandle.mIndex].mHandle == inObjectHandle, "Invalid Handle!");
	mCollisionObjects[inObjectHandle.mIndex] = {CollisionObject::ConstructParameters()};
	mCollisionObjects[inObjectHandle.mIndex].mAABB = {FLT_MAX, -FLT_MAX};
	mFreeCollisionObjectIndices.emplace_back(inObjectHandle.mIndex);

	mBVH.RemoveObject(inObjectHandle);
}

Transform2D CollisionWorld::MoveTo(
	const CollisionObjectHandle& inObjectHandle, Optional<Vec2> inPosition, Optional<float> inRotation,
	Optional<Vec2> inHalfSize)
{
	PROFILE_SCOPE(CollisionWorld::MoveToOptionals)
	gAssert(gIsMainThread());

	CollisionObject& object = mCollisionObjects[inObjectHandle.mIndex];

	gDebugIf(object.GetType() == CollisionObject::EType::Static, gLog(ELogType::Warning, "Trying to move a static object!"));

	Transform2D new_transform = object.GetTransform();
	if (inPosition.has_value())
		new_transform.mPosition = inPosition.value();
	if (inRotation.has_value())
		new_transform.mRotation = inRotation.value();
	if (inHalfSize.has_value())
		new_transform.mHalfSize = inHalfSize.value();

	MoveTo(inObjectHandle, new_transform);
	return new_transform;
}

void CollisionWorld::MoveTo(const CollisionObjectHandle& inObjectHandle, Transform2D& ioTransform)
{
	PROFILE_SCOPE(CollisionWorld::MoveTo)

	static THREADLOCAL Array<Pair<OnCollisionFunc, CollisionFuncParams>> collision_callback_functions;
	collision_callback_functions.clear();

	CollisionObject& object = mCollisionObjects[inObjectHandle.mIndex];

	gDebugIf(object.GetType() == CollisionObject::EType::Static, gLog(ELogType::Warning, "Trying to move a static object!"));

	Transform2D swept_shape = gComputeSweptShape(object.mTransform, ioTransform);
	AABB swept_shape_aabb = gComputeAABB(swept_shape);
	const Array<CollisionObjectHandle>& broadphase_collisions = mBVH.GetBroadphaseCollisions(swept_shape_aabb);

	for (const CollisionObjectHandle& collision : broadphase_collisions)
	{
		if (collision == inObjectHandle)
			continue;

		CollisionObject& other_object = GetCollisionObject(collision);
		CollisionInfo collision_info = gCollides(swept_shape, other_object.GetTransform());
		if (collision_info.mCollides)
		{
			CollisionFuncParams collision_func_params;
			collision_func_params.mOther = inObjectHandle;
			collision_func_params.mSelf = collision;
			collision_func_params.mCollisionInfo = collision_info;

			if (object.IsBlocking() && other_object.IsBlocking())
			{
				ioTransform.mPosition += collision_info.mDirection * collision_info.mDepth;
				swept_shape = gComputeSweptShape(object.mTransform, ioTransform);
			}

			collision_callback_functions.emplace_back(other_object.mOnCollisionFunction, collision_func_params);
		}
	}
	SetTransformInternal(inObjectHandle, ioTransform);

	for (auto& function : collision_callback_functions)
	{
		if (function.first == nullptr)
			continue;

		function.first(function.second);
	}
	for (auto& function : collision_callback_functions)
	{
		if (object.mOnCollisionFunction == nullptr)
			continue;

		gSwap(function.second.mSelf, function.second.mOther);
		object.mOnCollisionFunction(function.second);
	}
}

void CollisionWorld::TeleportPosition(const CollisionObjectHandle& inObjectHandle, const Vec2& inPosition)
{
	gAssert(gIsMainThread());
	Transform2D transform = mCollisionObjects[inObjectHandle.mIndex].GetTransform();
	transform.mPosition = inPosition;
	mCollisionObjects[inObjectHandle.mIndex].SetTransform(transform);
	mBVH.RefreshObject(inObjectHandle);
}

void CollisionWorld::TeleportTransform(const CollisionObjectHandle& inObjectHandle, const Transform2D& inTransform)
{
	gAssert(gIsMainThread());
	mCollisionObjects[inObjectHandle.mIndex].SetTransform(inTransform);
	mBVH.RefreshObject(inObjectHandle);
}

const Array<CollisionData>& CollisionWorld::CheckCollisions(const Transform2D mTransform)
{
	PROFILE_SCOPE(CollisionWorld::CheckCollision)
	gAssert(gIsMainThread());

	static THREADLOCAL Array<CollisionData> collision_data;
	collision_data.clear();

	AABB aabb = gComputeAABB(mTransform);
	const Array<CollisionObjectHandle>& broadphase_collisions = mBVH.GetBroadphaseCollisions(aabb);

	for (const CollisionObjectHandle& collision : broadphase_collisions)
	{
		CollisionObject& other_object = GetCollisionObject(collision);
		CollisionInfo collision_info = gCollides(mTransform, other_object.GetTransform());
		if (collision_info.mCollides)
		{
			CollisionData data;
			data.mHandle = collision;
			data.mCollisionInfo = collision_info;
			collision_data.emplace_back(data);
		}
	}

	return collision_data;
}

void CollisionWorld::DeserializeCollisionObject(const CollisionObjectHandle& inObjectHandle, const Json& inJson)
{
	gAssert(gIsMainThread());
	mCollisionObjects[inObjectHandle.mIndex].Deserialize(inJson);
	mBVH.RefreshObject(inObjectHandle);
}

CollisionObject& CollisionWorld::GetCollisionObject(const CollisionObjectHandle& inObjectHandle)
{
	gAssert(gIsMainThread());
	return mCollisionObjects[inObjectHandle.mIndex];
}

const CollisionObject& CollisionWorld::GetCollisionObject(const CollisionObjectHandle& inObjectHandle) const
{
	gAssert(gIsMainThread());
	return mCollisionObjects[inObjectHandle.mIndex];
}

void CollisionWorld::LoopCollisionObjects(const std::function<void(const CollisionObject&)>& inFunction)
{
	gAssert(gIsMainThread());
	for (const CollisionObject& object : mCollisionObjects)
	{
		if (object.mHandle.IsValid())
			inFunction(object);
	}
}

void CollisionWorld::SetTransformInternal(const CollisionObjectHandle& inObjectHandle, const Transform2D& inTransform)
{
	gAssert(gIsMainThread());
	CollisionObject& object = mCollisionObjects[inObjectHandle.mIndex];
	object.SetTransform(inTransform);
	mBVH.RefreshObject(inObjectHandle);
}

CollisionObjectHandle CollisionWorld::FindOrCreateCollisionObjectIndex(const CollisionObject::ConstructParameters& inConstructParameters)
{
	gAssert(gIsMainThread());

	CollisionObjectHandle handle;

	CollisionObject::ConstructParameters construct_parameters = inConstructParameters;

	if (mFreeCollisionObjectIndices.empty())
	{
		handle.mIndex = mCollisionObjects.size();
		construct_parameters.mHandle = handle;

		mCollisionObjects.emplace_back(construct_parameters);
		return handle;
	}

	uint64 index = mFreeCollisionObjectIndices.back();
	handle.mIndex = index;
	construct_parameters.mHandle = handle;
	mFreeCollisionObjectIndices.pop_back();
	mCollisionObjects[index] = {construct_parameters};
	return handle;
}
