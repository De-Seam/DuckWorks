#include "Precomp.h"
#include "Engine/Collision/CollisionObject.h"
#include "Engine/Collision/CollisionHelperFunctions.h"

Json CollisionObject::Serialize() const
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mType);
	JSON_SAVE(json, mShapeType);
	JSON_SAVE(json, mTransform);

	return json;
}

void CollisionObject::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	JSON_TRY_LOAD(inJson, mType);
	JSON_TRY_LOAD(inJson, mShapeType);
	JSON_TRY_LOAD(inJson, mTransform);

	SetTransform(mTransform);
}

CollisionObject::CollisionObject(const InitParams& inInitParams)
{
	mTransform = inInitParams.mTransform;
	mType = inInitParams.mType;
	CalculateAABB();
}

bool CollisionObject::Collides(const CollisionObject* inOther) const
{
	gAssert(inOther != nullptr, "Can't pass nullptr!");
	if (!gCollides(GetAABB(), inOther->GetAABB()))
		return false;

	CollisionInfo collision_info = gCollides(mTransform, inOther->GetTransform());
	return collision_info.mCollides;
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