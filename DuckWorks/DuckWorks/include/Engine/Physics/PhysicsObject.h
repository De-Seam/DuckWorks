#pragma once
// Core includes
#include "Core/CoreBase.h"
#include "Core/Utilities/RTTI.h"

class PhysicsObject : public RTTIBaseClass
{
	RTTI_CLASS(PhysicsObject, RTTIBaseClass)

	struct InitParams
	{
		fm::Transform2D mTransform;
	};
	PhysicsObject() = default;
	PhysicsObject(const InitParams& inInitParams);

	bool Collides(const PhysicsObject* inOther);

	void SetPosition(const fm::vec2& inPosition); ///< Set position. Will sweep and resolve collisions, new position will be set next frame
	void SetHalfSize(const fm::vec2& inHalfSize);
	void SetRotation(float inRotation);
	void SetTransform(const fm::Transform2D inTransform);

	const fm::vec2& GetPosition() const { return mTransform.position; }
	const fm::vec2& GetHalfSize() const { return mTransform.halfSize; }
	float GetRotation() const { return mTransform.rotation; }
	const fm::Transform2D& GetTransform() const { return mTransform; }


	void TeleportPosition(const fm::vec2& inPosition); ///< Teleport. Will instantly teleport, ignoring collision
	void TeleportHalfSize(const fm::vec2& inHalfSize); ///< Teleport. Will instantly teleport, ignoring collision
	void TeleportRotation(float inRotation); ///< Teleport. Will instantly teleport, ignoring collision
	void TeleportTransform(const fm::Transform2D inTransform); ///< Teleport. Will instantly teleport, ignoring collision
private:
	fm::Transform2D mTransform;

	fm::Transform2D mTargetTransform;

private:
	void Sync(); ///< Called by PhysicsWorld. Syncs transform to targettransform.

	friend class PhysicsWorld;
};