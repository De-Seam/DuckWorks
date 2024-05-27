#include "Precomp.h"
#include "Engine/Collision/CollisionObject.h"
#include "Engine/Collision/CollisionHelperFunctions.h"

RTTI_CLASS_DEFINITION(CollisionObject, ClassAllocator)

Json CollisionObject::Serialize()
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mType);
	JSON_SAVE(json, mBlocking);
	JSON_SAVE(json, mShapeType);
	JSON_SAVE(json, mTransform);

	return json;
}

void CollisionObject::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	JSON_TRY_LOAD(inJson, mType);
	JSON_TRY_LOAD(inJson, mBlocking);
	JSON_TRY_LOAD(inJson, mShapeType);
	JSON_TRY_LOAD(inJson, mTransform);

	SetTransform(mTransform);
}

CollisionObject::CollisionObject(const ConstructParameters& inConstructParameters) :
	Base(inConstructParameters),
	mHandle(inConstructParameters.mHandle),
	mTransform(inConstructParameters.mTransform),
	mType(inConstructParameters.mType),
	mBlocking(inConstructParameters.mBlocking),
	mEntity(inConstructParameters.mEntity)
{
	if (inConstructParameters.mOnCollisionFunction != nullptr)
		mOnCollisionFunction = inConstructParameters.mOnCollisionFunction;
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
