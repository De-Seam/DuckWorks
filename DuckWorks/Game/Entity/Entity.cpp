#include <Game/Entity/Entity.h>

// Game includes
#include <Game/World/World.h>

Entity::Entity(World* inWorld) : mWorld(inWorld)
{
	mEntityHandle = GetRegistry().create();
}

Entity::~Entity()
{
	GetRegistry().destroy(mEntityHandle);
}

entt::registry& Entity::GetRegistry() { return mWorld->GetRegistry(); }

const entt::registry& Entity::GetRegistry() const { return mWorld->GetRegistry(); }
