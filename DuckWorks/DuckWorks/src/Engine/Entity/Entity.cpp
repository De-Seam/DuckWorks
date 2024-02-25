#include "Precomp.h"
#include "Engine/Entity/Entity.h"

#include "Engine/World/World.h"

void Entity::GenerateNewEntityHandle(World* inWorld)
{
	mWorld = inWorld;
	entt::registry& registry = mWorld->GetRegistry();
	mEntityHandle = registry.create();
}
