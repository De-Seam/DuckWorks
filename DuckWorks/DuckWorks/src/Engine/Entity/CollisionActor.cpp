#include "Precomp.h"
#include "Engine/Entity/CollisionActor.h"

// Engine includes
#include "Engine/Collision/CollisionObject.h"
#include "Engine/Collision/CollisionWorld.h"
#include "Engine/Entity/Components.h"
#include "Engine/Entity/Components/CollisionComponent.h"
#include "Engine/World/World.h"

RTTI_CLASS_DEFINITION(CollisionActor, StandardAllocator)

Json CollisionActor::Serialize()
{
	Json json = Base::Serialize();

	return json;
}

void CollisionActor::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);
}

CollisionActor::CollisionActor(const ConstructParameters& inConstructParameters)
	: Base(inConstructParameters)
{
	CollisionComponent::ConstructParameters collision_component_params;
	collision_component_params.mCollisionObjectConstructParameters.mBlocking = true;
	collision_component_params.mCollisionObjectConstructParameters.mType = CollisionObject::EType::Static;

	AddComponent<CollisionComponent>(collision_component_params);
}

CollisionActor::~CollisionActor()
{
}

void CollisionActor::BeginPlay()
{
	Base::BeginPlay();
}

fm::Transform2D CollisionActor::MoveTo(Optional<fm::vec2> inPosition, Optional<float> inRotation, Optional<fm::vec2> inHalfSize)
{
	fm::Transform2D transform = GetTransform();
	if (inPosition)
		transform.position = *inPosition;
	if (inRotation)
		transform.rotation = *inRotation;
	if (inHalfSize)
		transform.halfSize = *inHalfSize;

	Base::SetTransform(transform);
	return transform;
}
