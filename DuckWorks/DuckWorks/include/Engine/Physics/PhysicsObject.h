#pragma once
// Core includes
#include "Core/CoreBase.h"
#include "Core/Utilities/RTTI.h"

// Engine includes
#include "Engine/Physics/PhysicsStructs.h"

class PhysicsObject : public RTTIBaseClass
{
	RTTI_CLASS(PhysicsObject, RTTIBaseClass)

	struct InitParams
	{
		fm::Transform2D mTransform;
	};

	PhysicsObject() = default;
	PhysicsObject(const InitParams& inInitParams);

	bool Collides(const PhysicsObject* inOther) const;

	enum class ShapeType : uint8
	{
		Box,
		Circle ///< As a circle, the radius is mTransform.halfSize.x, and mTransform.halfSize.y is ignored
	};

	[[deprecated]] void SetShapeType(ShapeType inShapeType);

	const fm::vec2& GetPosition() const { return mTransform.position; }
	const fm::vec2& GetHalfSize() const { return mTransform.halfSize; }
	float GetRotation() const { return mTransform.rotation; }
	const fm::Transform2D& GetTransform() const { return mTransform; }
	const AABB& GetAABB() const { return mAABB; }

private:
	fm::Transform2D mTransform;
	AABB mAABB;

	ShapeType mShapeType = ShapeType::Box;

private:
	void SetTransform(const fm::Transform2D& inTransform);
	void CalculateAABB();

	friend class PhysicsWorld;
};
