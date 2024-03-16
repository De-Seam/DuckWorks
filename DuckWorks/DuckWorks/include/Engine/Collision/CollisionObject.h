#pragma once
// Core includes
#include "Core/CoreBase.h"
#include "Core/Utilities/RTTI.h"

// Engine includes
#include "Engine/Collision/CollisionStructs.h"

class CollisionObject : public RTTIBaseClass
{
	RTTI_CLASS(CollisionObject, RTTIBaseClass)

	enum class Type : uint8
	{
		Static,
		Dynamic
	};

	struct InitParams
	{
		fm::Transform2D mTransform;
		Type mType = Type::Static;
	};

	CollisionObject() = default;
	CollisionObject(const InitParams& inInitParams);

	bool Collides(const CollisionObject* inOther) const;

	enum class ShapeType : uint8
	{
		Box,
		Circle ///< As a circle, the radius is mTransform.halfSize.x, and mTransform.halfSize.y is ignored
	};

	void SetType(Type inType) { mType = inType; }
	[[deprecated]] void SetShapeType(ShapeType inShapeType);

	const CollisionObjectHandle& GetHandle() const { return mHandle; }
	const fm::vec2& GetPosition() const { return mTransform.position; }
	const fm::vec2& GetHalfSize() const { return mTransform.halfSize; }
	float GetRotation() const { return mTransform.rotation; }
	const fm::Transform2D& GetTransform() const { return mTransform; }
	const AABB& GetAABB() const { return mAABB; }

	Type GetType() const { return mType; }

private:
	CollisionObjectHandle mHandle;

	fm::Transform2D mTransform;
	AABB mAABB;

	Type mType = Type::Static;

	ShapeType mShapeType = ShapeType::Box;

private:
	void SetTransform(const fm::Transform2D& inTransform);
	void CalculateAABB();

	friend class CollisionWorld;
};
