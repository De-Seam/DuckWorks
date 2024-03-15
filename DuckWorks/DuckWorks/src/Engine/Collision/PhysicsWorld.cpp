#include "Precomp.h"
#include "Engine/Collision/CollisionWorld.h"

// Engine includes
#include "Engine/Collision/CollisionHelperFunctions.h"
#include "Engine/Collision/CollisionStructs.h"

CollisionWorld::CollisionWorld()
{}

CollisionObjectHandle CollisionWorld::CreateCollisionObject()
{
	CollisionObjectHandle handle;
	handle.mIndex = FindOrCreateCollisionObjectIndex();
	return handle;
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
	CollisionObject& object = mCollisionObjects[inObjectHandle.mIndex];
	{
		ScopedMutexReadLock lock(mCollisionObjectsMutex);

		fm::Transform2D swept_shape = gComputeSweptShape(object.mTransform, inPosition, inRotation);
		for (uint64 i = 0; i < mCollisionObjects.size(); ++i)
		{
			if (i == inObjectHandle.mIndex)
				continue;

			CollisionObject& other_object = mCollisionObjects[i];

			if (gCollides(swept_shape, other_object.GetTransform()))
			{
				gLog("Collision detected");
			}
		}
	}

	{
		ScopedMutexWriteLock lock(mCollisionObjectsMutex);
		fm::Transform2D transform = object.GetTransform();
		transform.position = inPosition;
		object.SetTransform(transform);
	}
}

void CollisionWorld::SetTransform(const CollisionObjectHandle& inObjectHandle, const fm::Transform2D& inTransform)
{
	ScopedMutexWriteLock lock(mCollisionObjectsMutex);
	mCollisionObjects[inObjectHandle.mIndex].SetTransform(inTransform);
}

uint64 CollisionWorld::FindOrCreateCollisionObjectIndex()
{
	ScopedMutexWriteLock lock(mCollisionObjectsMutex);
	if (mFreeCollisionObjectIndices.empty())
	{
		mCollisionObjects.emplace_back();
		return mCollisionObjects.size() - 1;
	}
	uint64 index = mFreeCollisionObjectIndices.back();
	mFreeCollisionObjectIndices.pop_back();
	mCollisionObjects[index] = {};
	return index;
}
