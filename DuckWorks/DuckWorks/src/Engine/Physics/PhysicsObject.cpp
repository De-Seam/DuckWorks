#include "Precomp.h"
#include "Engine/Physics/PhysicsObject.h"
#include "Engine/Physics/PhysicsHelperFunctions.h"

RTTI_EMPTY_SERIALIZE_DEFINITION(PhysicsObject)

PhysicsObject::PhysicsObject(const PhysicsObject::InitParams& inInitParams)
{
	mTransform = inInitParams.mTransform;
	mTargetTransform = inInitParams.mTransform;
}

bool PhysicsObject::Collides(const PhysicsObject* inOther)
{
	return gCollides(mTransform, inOther->GetTransform());
}

void PhysicsObject::SetPosition(const fm::vec2& inPosition)
{
	mTargetTransform.position = inPosition;
}

void PhysicsObject::SetHalfSize(const fm::vec2& inHalfSize)
{
	mTargetTransform.halfSize = inHalfSize;
}

void PhysicsObject::SetRotation(float inRotation)
{
	mTargetTransform.rotation = inRotation;
}

void PhysicsObject::SetTransform(const fm::Transform2D inTransform)
{
	mTargetTransform = inTransform;
}

void PhysicsObject::TeleportPosition(const fm::vec2& inPosition)
{
	mTransform.position = inPosition;
	mTargetTransform.position = inPosition;
}

void PhysicsObject::TeleportHalfSize(const fm::vec2& inHalfSize)
{
	mTransform.halfSize = inHalfSize;
	mTargetTransform.halfSize = inHalfSize;
}

void PhysicsObject::TeleportRotation(float inRotation)
{
	mTransform.rotation = inRotation;
	mTargetTransform.rotation = inRotation;
}

void PhysicsObject::TeleportTransform(const fm::Transform2D inTransform)
{
	mTransform = inTransform;
	mTargetTransform = inTransform;
}

void PhysicsObject::Sync()
{

}
