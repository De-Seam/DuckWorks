#include "precomp.h"
#include "Engine/Entity/BaseEntity.h"

// Engine includes
#include "Engine/World/World.h"

BaseEntity::BaseEntity(World* inWorld)
{
	GenerateNewEntityHandle(inWorld);
}

BaseEntity::BaseEntity(entt::entity inHandle, World* inWorld)
{
	mWorld = inWorld;
	mEntityHandle = inHandle;
}

void BaseEntity::GenerateNewEntityHandle(World* inWorld)
{
	mWorld = inWorld;
	entt::registry& registry = mWorld->GetRegistry();
	mEntityHandle = registry.create();
}

entt::registry& BaseEntity::GetRegistry() const
{
	return mWorld->GetRegistry();
}
