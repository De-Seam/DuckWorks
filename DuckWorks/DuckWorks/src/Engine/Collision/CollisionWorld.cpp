#include "Precomp.h"
#include "Engine/Collision/CollisionWorld.h"

// Engine includes
#include "Engine/Collision/CollisionHelperFunctions.h"
#include "Engine/Collision/CollisionStructs.h"

RTTI_EMPTY_SERIALIZE_DEFINITION(CollisionWorld)

CollisionObjectHandle CollisionWorld::CreateCollisionObject(const CollisionObject::InitParams& inInitParams)
{
	return FindOrCreateCollisionObjectIndex(inInitParams);
}

void CollisionWorld::DestroyCollisionObject(const CollisionObjectHandle& inObjectHandle)
{
	ScopedMutexWriteLock lock(mCollisionObjectsMutex);
	mCollisionObjects[inObjectHandle.mIndex] = {};
	mFreeCollisionObjectIndices.emplace_back(inObjectHandle.mIndex);
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
	static THREADLOCAL Array<Pair<CollisionObjectHandle, CollisionInfo>> colliding_objects;
	colliding_objects.clear();

	fm::vec2 new_position = inPosition;
	CollisionObject& object = mCollisionObjects[inObjectHandle.mIndex];
	{
		ScopedMutexReadLock lock(mCollisionObjectsMutex);

		fm::Transform2D swept_shape = gComputeSweptShape(object.mTransform, new_position, inRotation);
		for (uint64 i = 0; i < mCollisionObjects.size(); ++i)
		{
			if (i == inObjectHandle.mIndex)
				continue;

			CollisionObject& other_object = mCollisionObjects[i];

			CollisionInfo collision_info = gCollides(swept_shape, other_object.GetTransform());
			if (collision_info.mCollides)
			{
				gLog("%v2", collision_info.mDirection);
				new_position += collision_info.mDirection * collision_info.mDepth;
				swept_shape = gComputeSweptShape(object.mTransform, new_position, inRotation);
			}
		}
	}

	{
		ScopedMutexWriteLock lock(mCollisionObjectsMutex);
		fm::Transform2D transform = object.GetTransform();
		transform.position = new_position;
		transform.rotation = inRotation;
		SetTransformInternal(inObjectHandle, transform);
		//object.SetTransform(transform);
	}
}

void CollisionWorld::TeleportTransform(const CollisionObjectHandle& inObjectHandle, const fm::Transform2D& inTransform)
{
	ScopedMutexWriteLock lock(mCollisionObjectsMutex);
	mCollisionObjects[inObjectHandle.mIndex].SetTransform(inTransform);
}

void CollisionWorld::DeserializeCollisionObject(const CollisionObjectHandle& inObjectHandle, const Json& inJson)
{
	ScopedMutexReadLock lock(mCollisionObjectsMutex);
	mCollisionObjects[inObjectHandle.mIndex].Deserialize(inJson);
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

}

CollisionObjectHandle CollisionWorld::FindOrCreateCollisionObjectIndex(const CollisionObject::InitParams& inInitParams)
{
	ScopedMutexWriteLock lock(mCollisionObjectsMutex);
	CollisionObjectHandle handle;
	if (mFreeCollisionObjectIndices.empty())
	{
		mCollisionObjects.emplace_back();
		handle.mIndex = mCollisionObjects.size() - 1;
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
