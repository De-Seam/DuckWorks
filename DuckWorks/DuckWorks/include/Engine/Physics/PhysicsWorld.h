#pragma once
// Core includes
#include "Core/CoreBase.h"
#include "Core/Utilities/RTTI.h"
#include "Core/Utilities/UID.h"

// Engine includes
#include "Engine/Physics/PhysicsObject.h"
#include "Engine/Physics/Grid.h"


struct PhysicsObjectHandle
{
	uint64 mIndex = UINT64_MAX;
	UID mUID = {};
};

class PhysicsWorld : public RTTIBaseClass
{
	RTTI_CLASS(PhysicsWorld, RTTIBaseClass)

	PhysicsWorld();

	PhysicsObjectHandle CreatePhysicsObject();
	void DestroyPhysicsObject(const PhysicsObjectHandle& inObjectHandle);

	void MoveTo(const PhysicsObjectHandle& inObjectHandle, const fm::vec2& inPosition);
	// Teleports object to the given position, does not check for collision.
	void SetTransform(const PhysicsObjectHandle& inObjectHandle, const fm::Transform2D& inTransform);

private:
	Array<PhysicsObject> mPhysicsObjects = {};
	Array<uint64> mFreePhysicsObjectIndices = {};

	Mutex mPhysicsObjectsMutex;

private:
	uint64 FindOrCreatePhysicsObjectIndex();
};
