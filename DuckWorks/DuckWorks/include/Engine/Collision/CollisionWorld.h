#pragma once
// Core includes
#include "Core/CoreBase.h"
#include "Core/RTTI/RTTI.h"
#include "Core/Utilities/UID.h"

// Engine includes
#include "Engine/Collision/CollisionObject.h"
#include "Engine/Collision/BVH/BVH.h"

class CollisionWorld : public RTTIBaseClass
{
	RTTI_CLASS(CollisionWorld, RTTIBaseClass, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	using Base::Base;

	CollisionWorld(const ConstructParameters& inConstructParameters = {});

	void BeginPlay();

	void Draw();
	void DrawCollision();
	void DrawBVH();

	CollisionObjectHandle CreateCollisionObject(const CollisionObject::ConstructParameters& inConstructParameters);
	void DestroyCollisionObject(const CollisionObjectHandle& inObjectHandle);

	// This function immediately checks for collision so don't call it multiple times on the same object if you can do it once!
	fm::Transform2D MoveTo(
		const CollisionObjectHandle& inObjectHandle, Optional<fm::vec2> inPosition, Optional<float> inRotation = NullOpt,
		Optional<fm::vec2> inHalfSize = NullOpt);
	void MoveTo(const CollisionObjectHandle& inObjectHandle, fm::Transform2D& ioTransform);
	// Teleports object to the given position, does not check for collision.
	void TeleportPosition(const CollisionObjectHandle& inObjectHandle, const fm::vec2& inPosition);
	void TeleportTransform(const CollisionObjectHandle& inObjectHandle, const fm::Transform2D& inTransform);

	const Array<CollisionData>& CheckCollisions(fm::Transform2D mTransform);

	void DeserializeCollisionObject(const CollisionObjectHandle& inObjectHandle, const Json& inJson);
	CollisionObject& GetCollisionObject(const CollisionObjectHandle& inObjectHandle);
	const CollisionObject& GetCollisionObject(const CollisionObjectHandle& inObjectHandle) const;
	///< Returns a locked mutex and a reference to the object. The mutex should be unlocked when done with it.
	void LoopCollisionObjects(const std::function<void(const CollisionObject&)>& inFunction);

private:
	Array<CollisionObject> mCollisionObjects = {};
	Array<uint64> mFreeCollisionObjectIndices = {};

	BVH mBVH;

private:
	// Sets the transform. Does NOT lock the mCollisionObjects mutex. That needs to be locked already.
	void SetTransformInternal(const CollisionObjectHandle& inObjectHandle, const fm::Transform2D& inTransform);
	CollisionObjectHandle FindOrCreateCollisionObjectIndex(const CollisionObject::ConstructParameters& inConstructParameters);
};
