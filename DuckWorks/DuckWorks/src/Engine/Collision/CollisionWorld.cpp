#include "Precomp.h"
#include "Engine/Collision/CollisionWorld.h"

// Engine includes
#include "Engine/Collision/CollisionHelperFunctions.h"
#include "Engine/Collision/CollisionStructs.h"

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

void CollisionWorld::MoveTo(const CollisionObjectHandle& inObjectHandle, const fm::vec2& inPosition)
{
	float rotation = mCollisionObjects[inObjectHandle.mIndex].GetRotation();
	MoveToAndRotate(inObjectHandle, inPosition, rotation);
}

void CollisionWorld::Rotate(const CollisionObjectHandle& inObjectHandle, float inRotation)
{
	fm::vec2 position = mCollisionObjects[inObjectHandle.mIndex].GetPosition();
	MoveToAndRotate(inObjectHandle, position, inRotation);
}

void CollisionWorld::MoveToAndRotate(const CollisionObjectHandle& inObjectHandle, const fm::vec2& inPosition, float inRotation)
{
	PROFILE_SCOPE(CollisionWorld::MoveToAndRotate)

	static THREADLOCAL Array<Pair<CollisionObjectHandle, CollisionInfo>> colliding_objects;
	colliding_objects.clear();

	fm::Transform2D new_transform;
	{
		ScopedMutexReadLock lock(mCollisionObjectsMutex);
		CollisionObject& object = mCollisionObjects[inObjectHandle.mIndex];
		new_transform = object.GetTransform();
		new_transform.position = inPosition;

		fm::Transform2D swept_shape = gComputeSweptShape(object.mTransform, new_transform.position, inRotation);
		AABB swept_shape_aabb = gComputeAABB(swept_shape);
		const Array<CollisionObjectHandle>& broadphase_collisions = mBVH.GetBroadphaseCollisions(swept_shape_aabb);
		for (const CollisionObjectHandle& collision : broadphase_collisions)
		{
			if (collision == inObjectHandle)
				continue;

			const CollisionObject& other_object = GetCollisionObject(collision);
			CollisionInfo collision_info = gCollides(swept_shape, other_object.GetTransform());
			if (collision_info.mCollides)
			{
				gLog("%v2", collision_info.mDirection);
				new_transform.position += collision_info.mDirection * collision_info.mDepth;
				swept_shape = gComputeSweptShape(object.mTransform, new_transform.position, inRotation);
			}
		}

		new_transform.rotation = inRotation;
	}
	SetTransformInternal(inObjectHandle, new_transform);
}

void CollisionWorld::TeleportTransform(const CollisionObjectHandle& inObjectHandle, const fm::Transform2D& inTransform)
{
	ScopedMutexWriteLock lock(mCollisionObjectsMutex);
	mCollisionObjects[inObjectHandle.mIndex].SetTransform(inTransform);
	mBVH.RefreshObject(inObjectHandle);
}

void CollisionWorld::DeserializeCollisionObject(const CollisionObjectHandle& inObjectHandle, const Json& inJson)
{
	ScopedMutexReadLock lock(mCollisionObjectsMutex);
	mCollisionObjects[inObjectHandle.mIndex].Deserialize(inJson);
	mBVH.RefreshObject(inObjectHandle);
}

const CollisionObject& CollisionWorld::GetCollisionObject(const CollisionObjectHandle& inObjectHandle)
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
	ScopedMutexWriteLock lock(mCollisionObjectsMutex);
	CollisionObjectHandle handle;
	if (mFreeCollisionObjectIndices.empty())
	{
		mCollisionObjects.emplace_back();
		handle.mIndex = mCollisionObjects.size() - 1;
		mCollisionObjects.back() = { inInitParams };
		mCollisionObjects.back().mHandle = handle;
		return handle;
	}
	uint64 index = mFreeCollisionObjectIndices.back();
	handle.mIndex = index;
	mFreeCollisionObjectIndices.pop_back();
	mCollisionObjects[index] = {inInitParams};
	mCollisionObjects[index].mHandle = handle;
	return handle;
}
