#include "Precomp.h"
#include "Engine/Physics/PhysicsObject.h"
#include "Engine/Physics/PhysicsHelperFunctions.h"

RTTI_EMPTY_SERIALIZE_DEFINITION(PhysicsObject)

PhysicsObject::PhysicsObject(const InitParams& inInitParams)
{
	mTransform = inInitParams.mTransform;
	CalculateAABB();
}

bool PhysicsObject::Collides(const PhysicsObject* inOther) const
{
	gAssert(inOther != nullptr, "Can't pass nullptr!");
	if (!gCollides(GetAABB(), inOther->GetAABB()))
		return false;

	return gCollides(mTransform, inOther->GetTransform());
}

void PhysicsObject::SetPosition(const fm::vec2& inPosition)
{
	mTransform.position = inPosition;
	CalculateAABB();
}

void PhysicsObject::SetHalfSize(const fm::vec2& inHalfSize)
{
	mTransform.halfSize = inHalfSize;
	CalculateAABB();
}

void PhysicsObject::SetRotation(float inRotation)
{
	mTransform.rotation = inRotation;
	CalculateAABB();
}

void PhysicsObject::SetTransform(const fm::Transform2D& inTransform)
{
	mTransform = inTransform;
	CalculateAABB();
}

void PhysicsObject::TeleportPosition(const fm::vec2& inPosition)
{
	mTransform.position = inPosition;
	CalculateAABB();
}

void PhysicsObject::TeleportHalfSize(const fm::vec2& inHalfSize)
{
	mTransform.halfSize = inHalfSize;
	CalculateAABB();
}

void PhysicsObject::TeleportRotation(float inRotation)
{
	mTransform.rotation = inRotation;
	CalculateAABB();
}

void PhysicsObject::TeleportTransform(const fm::Transform2D& inTransform)
{
	mTransform = inTransform;
	CalculateAABB();
}

void PhysicsObject::CalculateAABB()
{
	mAABB = gComputeAABB(mTransform);
}
