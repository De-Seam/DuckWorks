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

	void SetPosition(const fm::vec2& inPosition); ///< Set position. Will sweep and resolve collisions, new position will be set next frame
	void SetHalfSize(const fm::vec2& inHalfSize);
	void SetRotation(float inRotation);
	void SetTransform(const fm::Transform2D& inTransform);

	const fm::vec2& GetPosition() const { return mTransform.position; }
	const fm::vec2& GetHalfSize() const { return mTransform.halfSize; }
	float GetRotation() const { return mTransform.rotation; }
	const fm::Transform2D& GetTransform() const { return mTransform; }
	const AABB& GetAABB() const { return mAABB; }

	void TeleportPosition(const fm::vec2& inPosition); ///< Teleport. Will instantly teleport, ignoring collision
	void TeleportHalfSize(const fm::vec2& inHalfSize); ///< Teleport. Will instantly teleport, ignoring collision
	void TeleportRotation(float inRotation); ///< Teleport. Will instantly teleport, ignoring collision
	void TeleportTransform(const fm::Transform2D& inTransform); ///< Teleport. Will instantly teleport, ignoring collision
private:
	fm::Transform2D mTransform;
	AABB mAABB;

private:
	void CalculateAABB();

	friend class PhysicsWorld;
};
