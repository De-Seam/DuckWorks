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

private:
	Array<UniquePtr<PhysicsObject>> mPhysicsObject = {};
};