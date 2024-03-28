#include "Precomp.h"
#include "Engine/Entity/Actor.h"

// Engine includes
#include "Engine/World/World.h"

RTTI_CLASS_DECLARATION(Actor)

Json Actor::Serialize() const
{
	Json json = Base::Serialize();

	return json;
}

void Actor::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);
}


Actor::Actor(World* inWorld)
	: Entity(inWorld)
{
	AddComponent<TransformComponent>();
}
