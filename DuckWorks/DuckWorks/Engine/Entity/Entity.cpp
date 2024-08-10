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
	Base::OnAddedToWorld(inWorld);

	gAssert(mEntityHandle == entt::null);
	mEntityHandle = GetRegistry().create();
}

void Entity::OnRemovedFromWorld(World* inWorld) 
{
	gAssert(mEntityHandle != entt::null);
	GetRegistry().destroy(mEntityHandle);
	mEntityHandle = entt::null;

	Base::OnRemovedFromWorld(inWorld);
}

void Entity::OnTransformUpdated()
{
	GetComponent<TransformComponent>().SetTransform(GetWorldTransform(), GetRegistry(), mEntityHandle);
}
