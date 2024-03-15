#pragma once
// Core includes
#include "Core/CoreBase.h"
#include "Core/Utilities/RTTI.h"
#include "Core/Utilities/UID.h"

// Engine includes
#include "Engine/Collision/CollisionObject.h"
#include "Engine/Collision/Grid.h"

class CollisionWorld : public RTTIBaseClass
{
	RTTI_CLASS(CollisionWorld, RTTIBaseClass)

	CollisionWorld();

	CollisionObjectHandle CreateCollisionObject();
	void DestroyCollisionObject(const CollisionObjectHandle& inObjectHandle);

	// If you move AND rotate then use MoveToAndRotate to avoid checking collisions twice!
	void MoveTo(const CollisionObjectHandle& inObjectHandle, const fm::vec2& inPosition);
	void Rotate(const CollisionObjectHandle& inObjectHandle, float inRotation);
	void MoveToAndRotate(const CollisionObjectHandle& inObjectHandle, const fm::vec2& inPosition, float inNewRotation);
	// Teleports object to the given position, does not check for collision.
	void SetTransform(const CollisionObjectHandle& inObjectHandle, const fm::Transform2D& inTransform);

private:
	Array<CollisionObject> mCollisionObjects = {};
	Array<uint64> mFreeCollisionObjectIndices = {};

	Mutex mCollisionObjectsMutex;

private:
	uint64 FindOrCreateCollisionObjectIndex();
};
