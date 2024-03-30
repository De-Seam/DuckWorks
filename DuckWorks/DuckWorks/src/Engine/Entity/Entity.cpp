#include "Precomp.h"
#include "Engine/Entity/Entity.h"

RTTI_CLASS_DEFINITION(Entity)

Json Entity::Serialize() const
{
	Json json = Base::Serialize();

	return json;
}

void Entity::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);
}
