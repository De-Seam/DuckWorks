#include "Precomp.h"
#include "Engine/Entity/Actor.h"

// Engine includes
#include "Engine/World/World.h"

using namespace DC;

RTTI_CLASS_DEFINITION(Actor, StandardAllocator)

Json Actor::Serialize()
{
	Json json = Base::Serialize();

	return json;
}

void Actor::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);
}
