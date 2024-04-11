#include "Precomp.h"
#include "Engine/Entity/Entity.h"
#include "Engine/World/World.h"
#include "Engine/Collision/CollisionWorld.h"

RTTI_CLASS_DEFINITION(Entity)

Json Entity::Serialize()
{
	Json json = Base::Serialize();

	return json;
}

void Entity::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);
}

Entity::~Entity()
{}
