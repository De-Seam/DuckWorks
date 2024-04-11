#include "Precomp.h"
#include "Engine/Entity/Actor.h"

// Engine includes
#include "Engine/World/World.h"

RTTI_CLASS_DEFINITION(Actor)

Json Actor::Serialize()
{
	Json json = Base::Serialize();

	return json;
}

void Actor::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);
}

void Actor::Init(const Entity::InitParams& inInitParams)
{
	Base::Init(inInitParams);

	AddComponent<TransformComponent>();
}