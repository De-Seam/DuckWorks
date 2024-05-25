#pragma once
// Core includes
#include "Core/CoreBase.h"
#include "Core/Utilities/RTTI.h"

// Engine includes
#include "Engine/Collision/CollisionStructs.h"
#include "Engine/Entity/Entity.h"

class CollisionObject : public RTTIBaseClass
{
	RTTI_CLASS(CollisionObject, RTTIBaseClass)

public:
	enum class EType : uint8
	{
		Static,
		Dynamic
	};

	struct ConstructParameters : public Base::ConstructParameters
	{
		CollisionObjectHandle mHandle;

		fm::Transform2D mTransform;
		EType mType = EType::Static;
		bool mBlocking = true;

		WeakRef<Entity> mEntity = nullptr;

		OnCollisionFunc mOnCollisionFunction = nullptr;
	};

	CollisionObject(const ConstructParameters& inConstructParameters);

	bool Collides(const CollisionObject* inOther) const;

	enum class ShapeType : uint8
	{
		Box,
		Circle ///< As a circle, the radius is mTransform.halfSize.x, and mTransform.halfSize.y is ignored
	};

	void SetType(EType inType) { mType = inType; }
	void SetBlocking(bool inBlocking) { mBlocking = inBlocking; }
	[[deprecated]] void SetShapeType(ShapeType inShapeType);
	void SetEntityRef(const Ref<Entity>& inEntity) { mEntity = inEntity; }
	void SetOnCollisionFunc(const OnCollisionFunc& inOnCollisionFunc) { mOnCollisionFunction = inOnCollisionFunc; }

	const CollisionObjectHandle& GetHandle() const { return mHandle; }
	const fm::vec2& GetPosition() const { return mTransform.position; }
	const fm::vec2& GetHalfSize() const { return mTransform.halfSize; }
	float GetRotation() const { return mTransform.rotation; }
	const fm::Transform2D& GetTransform() const { return mTransform; }
	const AABB& GetAABB() const { return mAABB; }
	WeakRef<Entity> GetEntity() const { return mEntity; }

	EType GetType() const { return mType; }
	bool IsBlocking() const { return mBlocking; }

private:
	CollisionObjectHandle mHandle;

	fm::Transform2D mTransform;
	AABB mAABB;

	EType mType = EType::Static;
	bool mBlocking = true;

	ShapeType mShapeType = ShapeType::Box;

	OnCollisionFunc mOnCollisionFunction;

	WeakRef<Entity> mEntity = nullptr;

private:
	void SetTransform(const fm::Transform2D& inTransform);
	void CalculateAABB();

	friend class CollisionWorld;
};
