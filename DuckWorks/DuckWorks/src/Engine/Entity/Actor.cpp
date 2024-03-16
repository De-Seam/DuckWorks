#include "Precomp.h"
#include "Engine/Entity/Actor.h"

// Engine includes
#include "Engine/Renderer/Renderer.h"
#include "Engine/World/World.h"

// External includes
#include <External/box2d/box2d.h>

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
