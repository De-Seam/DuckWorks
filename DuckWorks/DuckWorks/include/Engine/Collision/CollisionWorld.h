#pragma once
// Core includes
#include "Core/CoreBase.h"
#include "Core/Utilities/RTTI.h"
#include "Core/Utilities/UID.h"

// Engine includes
#include "Engine/Collision/CollisionObject.h"
#include "Engine/Collision/Grid.h"
#include "Engine/Collision/BVH/BVH.h"

class CollisionWorld : public RTTIBaseClass
{
	RTTI_CLASS(CollisionWorld, RTTIBaseClass)

	CollisionWorld();
	void BeginPlay();

	CollisionObjectHandle CreateCollisionObject(const CollisionObject::InitParams& inInitParams);
	void DestroyCollisionObject(const CollisionObjectHandle& inObjectHandle);

	// If you move AND rotate then use MoveToAndRotate to avoid checking collisions twice!
	void MoveTo(const CollisionObjectHandle& inObjectHandle, const fm::vec2& inPosition);
	void Rotate(const CollisionObjectHandle& inObjectHandle, float inRotation);
	void MoveToAndRotate(const CollisionObjectHandle& inObjectHandle, const fm::vec2& inPosition, float inRotation);
	// Teleports object to the given position, does not check for collision.
	void TeleportTransform(const CollisionObjectHandle& inObjectHandle, const fm::Transform2D& inTransform);

	void DeserializeCollisionObject(const CollisionObjectHandle& inObjectHandle, const Json& inJson);
	const CollisionObject& GetCollisionObject(const CollisionObjectHandle& inObjectHandle);
	void LoopCollisionObjects(const std::function<void(const CollisionObject&)>& inFunction);

private:
	Array<CollisionObject> mCollisionObjects = {};
	Array<uint64> mFreeCollisionObjectIndices = {};

	Mutex mCollisionObjectsMutex;

	BVH mBVH;

private:
	// Sets the transform. Does NOT lock the mCollisionObjects mutex. That needs to be locked already.
	void SetTransformInternal(const CollisionObjectHandle& inObjectHandle, const fm::Transform2D& inTransform);
	CollisionObjectHandle FindOrCreateCollisionObjectIndex(const CollisionObject::InitParams& inInitParams);
};
