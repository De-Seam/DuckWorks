#include <Precomp.h>
#include <Engine/Entity/Entity.h>

// Engine includes
#include <Engine/Entity/Components/TransformComponent.h>
#include <Engine/World/World.h>


entt::registry& Entity::GetRegistry()
{
	gAssert(GetWorld() != nullptr);
	return GetWorld()->GetRegistry();
}

const entt::registry& Entity::GetRegistry() const 
{
	gAssert(GetWorld() != nullptr);
	return GetWorld()->GetRegistry();
}

void Entity::OnAddedToWorld(World* inWorld)
{
	gAssert(mWorld == nullptr);
	gAssert(mEntityHandle == entt::null);
	mWorld = inWorld;
	mEntityHandle = GetRegistry().create();
}

void Entity::OnRemovedFromWorld() 
{
	gAssert(mEntityHandle != entt::null);
	gAssert(mWorld != nullptr);
	GetRegistry().destroy(mEntityHandle);
	mEntityHandle = entt::null;
	mWorld = nullptr;
}
