#include "Precomp.h"
#include "Engine/Physics/PhysicsWorld.h"

// Engine includes
#include "Engine/Physics/PhysicsHelperFunctions.h"
#include "Engine/Physics/PhysicsStructs.h"

PhysicsWorld::PhysicsWorld()
{}

PhysicsObject* PhysicsWorld::CreatePhysicsObject()
{
	return mPhysicsObject.emplace_back(std::make_unique<PhysicsObject>()).get();
}

void PhysicsWorld::DestroyPhysicsObject(PhysicsObject* inObject)
{
	std::erase_if(mPhysicsObject, [inObject](const std::unique_ptr<PhysicsObject>& obj) { return obj.get() == inObject; });
}

void PhysicsWorld::MoveTo(PhysicsObject* inObject, const fm::vec2& inPosition)
{
	fm::Transform2D swept_shape = gComputeSweptShape(inObject->mTransform, inPosition);
	for (auto& object : mPhysicsObject)
	{
		if (object.get() == inObject)
			continue;

		if (gCollides(swept_shape, object->GetTransform()))
		{
			gLog("Collision detected");
		}
	}
}

void PhysicsWorld::SetTransform(PhysicsObject* inObject, const fm::Transform2D& inTransform)
{}
