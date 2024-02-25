#include "precomp.h"
#include "Engine/Entity/BaseEntity.h"

// Engine includes
#include "Engine/World/World.h"

BaseEntity::BaseEntity(World* inWorld)
{
	mWorld = inWorld;
	entt::registry& registry = mWorld->GetRegistry();
	mEntityHandle = registry.create();
}

BaseEntity::BaseEntity(entt::entity inHandle, World* inWorld)
{
	mWorld = inWorld;
	mEntityHandle = inHandle;
}

entt::registry& BaseEntity::GetRegistry() const
{
	return mWorld->GetRegistry();
}
