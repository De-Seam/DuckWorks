#include "Precomp.h"
#include "Engine/Entity/Entity.h"
#include "Engine/World/World.h"
#include "Engine/Collision/CollisionWorld.h"

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

Entity::~Entity()
{
	if (HasComponent<CollisionComponent>())
	{
		CollisionComponent& collision_component = GetComponent<CollisionComponent>();
		GetWorld()->GetCollisionWorld()->DestroyCollisionObject(collision_component.mCollisionObjectHandle);
	}
}
