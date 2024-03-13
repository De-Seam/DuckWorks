#pragma once
// Core includes
#include "Core/CoreBase.h"
#include "Core/Utilities/RTTI.h"

// Engine includes
#include "Engine/Physics/PhysicsObject.h"

class PhysicsWorld : public RTTIBaseClass
{
	RTTI_CLASS(PhysicsWorld, RTTIBaseClass)

	PhysicsWorld();

	PhysicsObject* CreatePhysicsObject();
	void DestroyPhysicsObject(PhysicsObject* inObject);

	void MoveTo(PhysicsObject* inObject, const fm::vec2& inPosition);
	void SetTransform(PhysicsObject* inObject, const fm::Transform2D& inTransform);

private:
	Array<UniquePtr<PhysicsObject>> mPhysicsObject = {};
};
