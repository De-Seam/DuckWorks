#include <Precomp.h>
#include <Engine/Entity/Entity.h>

// Engine includes
#include <Engine/Entity/Components/TransformComponent.h>
#include <Engine/World/World.h>

void Entity::SetTransform(const Transform2D& inTransform) 
{
	GetComponent<TransformComponent>().SetTransform(inTransform, GetRegistry(), mEntityHandle);
}

const Transform2D& Entity::GetTransform() const 
{
	return GetComponent<TransformComponent>().GetTransform();
}

void Entity::OnAddedToWorld(World* inWorld) 
{
	gAssert(mWorld == nullptr);
	mWorld = inWorld;
	mEntityHandle = GetRegistry().create();
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

const entt::registry& Entity::GetRegistry() const 
{
	gAssert(mWorld != nullptr);
	return mWorld->GetRegistry();
}
