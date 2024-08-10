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

void Entity::OnAddedToParent() 
{
	Base::OnAddedToParent();

	gAssert(mEntityHandle == entt::null);
	mEntityHandle = GetRegistry().create();
}

void Entity::OnRemovedFromParent() 
{
	gAssert(mEntityHandle != entt::null);
	GetRegistry().destroy(mEntityHandle);
	mEntityHandle = entt::null;

	Base::OnRemovedFromParent();
}

void Entity::OnTransformUpdated()
{
	if (GetWorld() == nullptr)
		return;
	GetComponent<TransformComponent>().SetTransform(GetWorldTransform(), GetRegistry(), mEntityHandle);
}
