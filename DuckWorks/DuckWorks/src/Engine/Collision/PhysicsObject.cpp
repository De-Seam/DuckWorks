#include "Precomp.h"
#include "Engine/Collision/CollisionObject.h"
#include "Engine/Collision/CollisionHelperFunctions.h"

RTTI_EMPTY_SERIALIZE_DEFINITION(CollisionObject)

CollisionObject::CollisionObject(const InitParams& inInitParams)
{
	mTransform = inInitParams.mTransform;
	CalculateAABB();
}

bool CollisionObject::Collides(const CollisionObject* inOther) const
{
	gAssert(inOther != nullptr, "Can't pass nullptr!");
	if (!gCollides(GetAABB(), inOther->GetAABB()))
		return false;

	return gCollides(mTransform, inOther->GetTransform());
}

void CollisionObject::SetShapeType(ShapeType inShapeType)
{
	mShapeType = inShapeType;
	CalculateAABB();
}

void CollisionObject::SetTransform(const fm::Transform2D& inTransform)
{
	mTransform = inTransform;
	CalculateAABB();
}

void CollisionObject::CalculateAABB()
{
	mAABB = gComputeAABB(mTransform);
}
