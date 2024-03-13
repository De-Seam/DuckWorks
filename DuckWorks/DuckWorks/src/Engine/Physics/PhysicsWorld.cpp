#include "Precomp.h"
#include "Engine/Physics/PhysicsWorld.h"

// Engine includes
#include "Engine/Physics/PhysicsHelperFunctions.h"
#include "Engine/Physics/PhysicsStructs.h"

PhysicsWorld::PhysicsWorld()
{}

PhysicsObjectHandle PhysicsWorld::CreatePhysicsObject()
{
	PhysicsObjectHandle handle;
	handle.mIndex = FindOrCreatePhysicsObjectIndex();
	return handle;
}

void PhysicsWorld::DestroyPhysicsObject(const PhysicsObjectHandle& inObjectHandle)
{
	ScopedMutexWriteLock lock(mPhysicsObjectsMutex);
	mPhysicsObjects[inObjectHandle.mIndex] = {};
	mFreePhysicsObjectIndices.emplace_back(inObjectHandle.mIndex);
}

void PhysicsWorld::MoveTo(const PhysicsObjectHandle& inObjectHandle, const fm::vec2& inPosition)
{
	PhysicsObject& object = mPhysicsObjects[inObjectHandle.mIndex];
	{
		ScopedMutexReadLock lock(mPhysicsObjectsMutex);

		fm::Transform2D swept_shape = gComputeSweptShape(object.mTransform, inPosition);
		for (uint64 i = 0; i < mPhysicsObjects.size(); ++i)
		{
			if (i == inObjectHandle.mIndex)
				continue;

			PhysicsObject& other_object = mPhysicsObjects[i];

			if (gCollides(swept_shape, other_object.GetTransform()))
			{
				gLog("Collision detected");
			}
		}
	}

	{
		ScopedMutexWriteLock lock(mPhysicsObjectsMutex);
		fm::Transform2D transform = object.GetTransform();
		transform.position = inPosition;
		object.SetTransform(transform);
	}
}

void PhysicsWorld::SetTransform(const PhysicsObjectHandle& inObjectHandle, const fm::Transform2D& inTransform)
{
	ScopedMutexWriteLock lock(mPhysicsObjectsMutex);
	mPhysicsObjects[inObjectHandle.mIndex].SetTransform(inTransform);
}

uint64 PhysicsWorld::FindOrCreatePhysicsObjectIndex()
{
	ScopedMutexWriteLock lock(mPhysicsObjectsMutex);
	if (mFreePhysicsObjectIndices.empty())
	{
		mPhysicsObjects.emplace_back();
		return mPhysicsObjects.size() - 1;
	}
	uint64 index = mFreePhysicsObjectIndices.back();
	mFreePhysicsObjectIndices.pop_back();
	mPhysicsObjects[index] = {};
	return index;
}
