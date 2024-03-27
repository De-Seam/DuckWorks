#include "Precomp.h"
#include "Engine/Entity/CollisionActor.h"

// Engine includes
#include "Engine/Collision/CollisionObject.h"
#include "Engine/Collision/CollisionWorld.h"
#include "Engine/World/WOrld.h"

Json CollisionActor::Serialize() const
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mRelativeTransform);

	return json;
}

void CollisionActor::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	JSON_TRY_LOAD(inJson, mRelativeTransform);
}

CollisionActor::CollisionActor(World* inWorld)
	: Base(inWorld)
{
	CollisionObject::InitParams params;
	params.mBlocking = true;
	params.mTransform = GetTransform();
	params.mType = CollisionObject::EType::Static;
	CollisionObjectHandle handle = GetWorld()->GetCollisionWorld()->CreateCollisionObject(params);
	AddComponent<CollisionComponent>(handle);
}

fm::Transform2D CollisionActor::MoveTo(Optional<fm::vec2> inPosition, Optional<float> inRotation, Optional<fm::vec2> inHalfSize)
{
	if (inPosition.has_value())
		inPosition.value() -= mRelativeTransform.position;
	if (inRotation.has_value())
		inRotation.value() -= mRelativeTransform.rotation;
	if (inHalfSize.has_value())
		inHalfSize.value() -= mRelativeTransform.halfSize;

	const CollisionObjectHandle& handle = GetComponent<CollisionComponent>().mCollisionObjectHandle;
	fm::Transform2D transform = GetWorld()->GetCollisionWorld()->MoveTo(handle, inPosition, inRotation, inHalfSize);
	transform = transform + mRelativeTransform;
	Base::SetTransform(transform);
	return transform;
}

void CollisionActor::TeleportPosition(const fm::vec2& inPosition)
{
	const CollisionObjectHandle& handle = GetComponent<CollisionComponent>().mCollisionObjectHandle;
	GetWorld()->GetCollisionWorld()->TeleportPosition(handle, inPosition - mRelativeTransform.position);
	Base::SetPosition(inPosition + mRelativeTransform.position);
}

void CollisionActor::TeleportTransform(const fm::Transform2D& inTransform)
{
	const CollisionObjectHandle& handle = GetComponent<CollisionComponent>().mCollisionObjectHandle;
	GetWorld()->GetCollisionWorld()->TeleportTransform(handle, inTransform - mRelativeTransform);
	Base::SetTransform(inTransform + mRelativeTransform);
}
