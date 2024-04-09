#include "Precomp.h"
#include "Engine/Collision/CollisionWorld.h"

// Engine includes
#include "Engine/Collision/CollisionHelperFunctions.h"
#include "Engine/Collision/CollisionStructs.h"
#include "Engine/Debug/DebugUIFunctions.h"

RTTI_CLASS_DEFINITION(CollisionWorld)

RTTI_EMPTY_SERIALIZE_DEFINITION(CollisionWorld)

CollisionWorld::CollisionWorld()
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
		gDrawAABB(inObject.GetAABB(), {1.f, 1.f, 1.f, 1.f});
	});
}

void CollisionWorld::DrawBVH()
{
	mBVH.Draw();
}

CollisionObjectHandle CollisionWorld::CreateCollisionObject(const CollisionObject::InitParams& inInitParams)
{
	PROFILE_SCOPE(CollisionWorld::CreateCollisionObject)

	CollisionObjectHandle handle = FindOrCreateCollisionObjectIndex(inInitParams);
	mBVH.AddObject(handle);
	return handle;
}

void CollisionWorld::DestroyCollisionObject(const CollisionObjectHandle& inObjectHandle)
{
	PROFILE_SCOPE(CollisionWorld::DestroyCollisionObject)

	ScopedMutexWriteLock lock(mCollisionObjectsMutex);
	gAssert(mCollisionObjects[inObjectHandle.mIndex].mHandle == inObjectHandle, "Invalid Handle!");
	mCollisionObjects[inObjectHandle.mIndex] = {};
	mFreeCollisionObjectIndices.emplace_back(inObjectHandle.mIndex);

	mBVH.RemoveObject(inObjectHandle);
}

fm::Transform2D CollisionWorld::MoveTo(
	const CollisionObjectHandle& inObjectHandle, Optional<fm::vec2> inPosition, Optional<float> inRotation,
	Optional<fm::vec2> inHalfSize)
{
	PROFILE_SCOPE(CollisionWorld::MoveTo)

	static THREADLOCAL Array<Pair<OnCollisionFunc, CollisionFuncParams>> collision_callback_functions;
	collision_callback_functions.clear();

	fm::Transform2D new_transform;
	{
		ScopedMutexReadLock lock(mCollisionObjectsMutex);
		CollisionObject& object = mCollisionObjects[inObjectHandle.mIndex];

		gDebugIf(object.GetType() == CollisionObject::EType::Static, gLog(LogType::Warning, "Trying to move a static object!"));

		new_transform = object.GetTransform();
		if (inPosition.has_value())
			new_transform.position = inPosition.value();
		if (inRotation.has_value())
			new_transform.rotation = inRotation.value();
		if (inHalfSize.has_value())
			new_transform.halfSize = inHalfSize.value();

		fm::Transform2D swept_shape = gComputeSweptShape(object.mTransform, new_transform);
		AABB swept_shape_aabb = gComputeAABB(swept_shape);
		const Array<CollisionObjectHandle>& broadphase_collisions = mBVH.GetBroadphaseCollisions(swept_shape_aabb);

		for (const CollisionObjectHandle& collision : broadphase_collisions)
		{
			if (collision == inObjectHandle)
				continue;

			CollisionObject& other_object = GetCollisionObjectNoMutex(collision);
			CollisionInfo collision_info = gCollides(swept_shape, other_object.GetTransform());
			if (collision_info.mCollides)
			{
				CollisionFuncParams collision_func_params;
				collision_func_params.mOther = inObjectHandle;
				collision_func_params.mSelf = collision;
				collision_func_params.mCollisionInfo = collision_info;

				if (object.IsBlocking() && other_object.IsBlocking())
				{
					new_transform.position += collision_info.mDirection * collision_info.mDepth;
					swept_shape = gComputeSweptShape(object.mTransform, new_transform);
				}

				collision_callback_functions.emplace_back(other_object.mOnCollisionFunction, collision_func_params);
			}
		}
	}
	SetTransformInternal(inObjectHandle, new_transform);

	ScopedMutexReadLock lock(mCollisionObjectsMutex);
	CollisionObject& object = mCollisionObjects[inObjectHandle.mIndex];
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

		fm::swap(function.second.mSelf, function.second.mOther);
		object.mOnCollisionFunction(function.second);
	}

	return new_transform;
}

void CollisionWorld::TeleportPosition(const CollisionObjectHandle& inObjectHandle, const fm::vec2& inPosition)
{
	ScopedMutexReadLock lock(mCollisionObjectsMutex);
	fm::Transform2D transform = mCollisionObjects[inObjectHandle.mIndex].GetTransform();
	transform.position = inPosition;
	mCollisionObjects[inObjectHandle.mIndex].SetTransform(transform);
	mBVH.RefreshObject(inObjectHandle);
}

void CollisionWorld::TeleportTransform(const CollisionObjectHandle& inObjectHandle, const fm::Transform2D& inTransform)
{
	ScopedMutexReadLock lock(mCollisionObjectsMutex);
	mCollisionObjects[inObjectHandle.mIndex].SetTransform(inTransform);
	mBVH.RefreshObject(inObjectHandle);
}

const Array<CollisionData>& CollisionWorld::CheckCollisions(const fm::Transform2D mTransform)
{
	PROFILE_SCOPE(CollisionWorld::CheckCollision)

	static THREADLOCAL Array<CollisionData> collision_data;
	collision_data.clear();

	AABB aabb = gComputeAABB(mTransform);
	const Array<CollisionObjectHandle>& broadphase_collisions = mBVH.GetBroadphaseCollisions(aabb);

	ScopedMutexReadLock lock(mCollisionObjectsMutex);
	for (const CollisionObjectHandle& collision : broadphase_collisions)
	{
		CollisionObject& other_object = GetCollisionObjectNoMutex(collision);
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
	ScopedMutexReadLock lock(mCollisionObjectsMutex);
	mCollisionObjects[inObjectHandle.mIndex].Deserialize(inJson);
	mBVH.RefreshObject(inObjectHandle);
}

CollisionObjectWrapper CollisionWorld::GetCollisionObject(const CollisionObjectHandle& inObjectHandle)
{
	mCollisionObjectsMutex.ReadLock();
	return {mCollisionObjectsMutex, &GetCollisionObjectNoMutex(inObjectHandle), true};
}

CollisionObject& CollisionWorld::GetCollisionObjectNoMutex(const CollisionObjectHandle& inObjectHandle)
{
	return mCollisionObjects[inObjectHandle.mIndex];
}

void CollisionWorld::LoopCollisionObjects(const std::function<void(const CollisionObject&)>& inFunction)
{
	ScopedMutexReadLock lock(mCollisionObjectsMutex);
	for (const CollisionObject& object : mCollisionObjects)
	{
		inFunction(object);
	}
}

void CollisionWorld::SetTransformInternal(const CollisionObjectHandle& inObjectHandle, const fm::Transform2D& inTransform)
{
	ScopedMutexReadLock lock(mCollisionObjectsMutex);
	CollisionObject& object = mCollisionObjects[inObjectHandle.mIndex];
	object.SetTransform(inTransform);
	mBVH.RefreshObject(inObjectHandle);
}

CollisionObjectHandle CollisionWorld::FindOrCreateCollisionObjectIndex(const CollisionObject::InitParams& inInitParams)
{
	CollisionObjectHandle handle;
	if (mFreeCollisionObjectIndices.empty())
	{
		ScopedMutexWriteLock lock(mCollisionObjectsMutex);
		mCollisionObjects.emplace_back();
		handle.mIndex = mCollisionObjects.size() - 1;
		mCollisionObjects.back() = {inInitParams};
		mCollisionObjects.back().mHandle = handle;
		return handle;
	}

	ScopedMutexReadLock lock(mCollisionObjectsMutex);
	uint64 index = mFreeCollisionObjectIndices.back();
	handle.mIndex = index;
	mFreeCollisionObjectIndices.pop_back();
	mCollisionObjects[index] = {inInitParams};
	mCollisionObjects[index].mHandle = handle;
	return handle;
}
