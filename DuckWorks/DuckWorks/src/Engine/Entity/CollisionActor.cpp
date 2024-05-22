#include "Precomp.h"
#include "Engine/Entity/CollisionActor.h"

// Engine includes
#include "Engine/Collision/CollisionObject.h"
#include "Engine/Collision/CollisionWorld.h"
#include "Engine/Entity/Components.h"
#include "Engine/World/WOrld.h"

RTTI_CLASS_DEFINITION(CollisionActor)

Json CollisionActor::Serialize()
{
	Json json = Base::Serialize();

	return json;
}

void CollisionActor::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);
}

CollisionActor::CollisionActor()
{
	
}

CollisionActor::~CollisionActor()
{
	Array<CollisionComponent*> collision_components = GetComponentsOfType<CollisionComponent>();
	World* world = GetWorld();
	CollisionWorld* collision_world = world->GetCollisionWorld();

	LoopOverComponents<CollisionComponent>([collision_world](CollisionComponent& inCollisionComponent)
	{
		collision_world->DestroyCollisionObject(inCollisionComponent.mCollisionObjectHandle);
	});
}

void CollisionActor::Init(const InitParams& inInitParams)
{
	Base::Init(inInitParams);
	
	CollisionObject::InitParams params;
	params.mBlocking = true;
	params.mTransform = GetTransform();
	params.mType = CollisionObject::EType::Static;
	params.mEntity = this;
	CollisionObjectHandle handle = GetWorld()->GetCollisionWorld()->CreateCollisionObject(params);
	AddComponent<CollisionComponent>(handle);
}

void CollisionActor::BeginPlay()
{
	Base::BeginPlay();
}

fm::Transform2D CollisionActor::MoveTo(Optional<fm::vec2> inPosition, Optional<float> inRotation, Optional<fm::vec2> inHalfSize)
{
	fm::Transform2D transform = GetTransform();
	LoopOverComponents<CollisionComponent>([&](const CollisionComponent& inCollisionComponent)
	{
		fm::Transform2D new_transform = GetWorld()->GetCollisionWorld()->MoveTo(inCollisionComponent.mCollisionObjectHandle, inPosition, inRotation, inHalfSize);
		transform.position = new_transform.position;
		transform.rotation = new_transform.rotation;
	});
	Base::SetTransform(transform);
	return transform;
}

void CollisionActor::TeleportPosition(const fm::vec2& inPosition)
{
	Base::SetPosition(inPosition);

	LoopOverComponents<CollisionComponent>([&](const CollisionComponent& inCollisionComponent)
	{
		GetWorld()->GetCollisionWorld()->TeleportPosition(inCollisionComponent.mCollisionObjectHandle, inPosition);
	});
}

void CollisionActor::TeleportTransform(const fm::Transform2D& inTransform)
{
	Base::SetTransform(inTransform);

	LoopOverComponents<CollisionComponent>([&](const CollisionComponent& inCollisionComponent)
	{
		GetWorld()->GetCollisionWorld()->TeleportTransform(inCollisionComponent.mCollisionObjectHandle, inTransform);
	});
}
