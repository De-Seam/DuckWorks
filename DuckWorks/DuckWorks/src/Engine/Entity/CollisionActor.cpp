#include "Precomp.h"
#include "Engine/Entity/CollisionActor.h"

// Engine includes
#include "Engine/Collision/CollisionObject.h"
#include "Engine/Collision/CollisionWorld.h"
#include "Engine/World/WOrld.h"

Json CollisionActor::Serialize() const
{
	Json json = Base::Serialize();

	return json;
}

void CollisionActor::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);
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

void CollisionActor::SetTransform(const fm::Transform2D& inTransform)
{
	const CollisionObjectHandle& handle = GetComponent<CollisionComponent>().mCollisionObjectHandle;
	fm::Transform2D transform = GetWorld()->GetCollisionWorld()->MoveToAndRotate(handle, inTransform.position, inTransform.rotation);
	transform.halfSize = inTransform.halfSize;

	Base::SetTransform(transform);
}

void CollisionActor::SetPosition(const fm::vec2& inPosition)
{
	const CollisionObjectHandle& handle = GetComponent<CollisionComponent>().mCollisionObjectHandle;
	fm::vec2 position = GetWorld()->GetCollisionWorld()->MoveTo(handle, inPosition);
	
	Base::SetPosition(position);
}

void CollisionActor::SetHalfSize(const fm::vec2& inHalfSize)
{
	Base::SetHalfSize(inHalfSize);
}

void CollisionActor::SetRotation(float inRotation)
{
	const fm::vec2& position = Base::GetPosition();
	const CollisionObjectHandle& handle = GetComponent<CollisionComponent>().mCollisionObjectHandle;
	fm::Transform2D transform = GetWorld()->GetCollisionWorld()->MoveToAndRotate(handle, position, inRotation);

	Base::SetTransform(transform);
}
