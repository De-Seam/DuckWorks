#include "Precomp.h"
#include "Engine/Entity/CollisionActor.h"

// Engine includes
#include "Engine/Collision/CollisionObject.h"
#include "Engine/Collision/CollisionWorld.h"
#include "Engine/World/WOrld.h"

RTTI_CLASS_DEFINITION(CollisionActor)

Json CollisionActor::Serialize()
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

CollisionActor::~CollisionActor()
{
	MutexReadProtectedPtr<CollisionComponent> collision_component = GetComponent<CollisionComponent>();
	World* world = GetWorld();
	CollisionWorld* collision_world = world->GetCollisionWorld();
	collision_world->DestroyCollisionObject(collision_component->mCollisionObjectHandle);
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
	if (inPosition.has_value())
		inPosition.value() -= mRelativeTransform.position;
	if (inRotation.has_value())
		inRotation.value() -= mRelativeTransform.rotation;
	if (inHalfSize.has_value())
		inHalfSize.value() -= mRelativeTransform.halfSize;

	fm::Transform2D transform;
	{
		MutexReadProtectedPtr<CollisionComponent> collision_component = GetComponent<CollisionComponent>();
		transform = GetWorld()->GetCollisionWorld()->MoveTo(collision_component->mCollisionObjectHandle, inPosition, inRotation, inHalfSize);
	}
	transform = transform + mRelativeTransform;
	Base::SetTransform(transform);
	return transform;
}

void CollisionActor::TeleportPosition(const fm::vec2& inPosition)
{
	MutexReadProtectedPtr<CollisionComponent> collision_component = GetComponent<CollisionComponent>();
	GetWorld()->GetCollisionWorld()->TeleportPosition(collision_component->mCollisionObjectHandle, inPosition - mRelativeTransform.position);
	Base::SetPosition(inPosition + mRelativeTransform.position);
}

void CollisionActor::TeleportTransform(const fm::Transform2D& inTransform)
{
	MutexReadProtectedPtr<CollisionComponent> collision_component = GetComponent<CollisionComponent>();
	GetWorld()->GetCollisionWorld()->TeleportTransform(collision_component->mCollisionObjectHandle, inTransform - mRelativeTransform);
	Base::SetTransform(inTransform + mRelativeTransform);
}
