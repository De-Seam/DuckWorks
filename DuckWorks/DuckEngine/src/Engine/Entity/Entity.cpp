#include <Engine/Precomp.h>
#include <Engine/Entity/Entity.h>

// Engine includes
#include <Engine/World/World.h>

void Entity::OnAddedToWorld(World* inWorld) 
{
	gAssert(mWorld == nullptr);
	mWorld = inWorld;
}

void Entity::OnRemovedFromWorld(World* inWorld) 
{
	(void)inWorld;	
	gAssert(mWorld == inWorld);
	mWorld = nullptr;
}

entt::registry& Entity::GetRegistry() 
{
	gAssert(mWorld != nullptr);
	return mWorld->GetRegistry();
}
