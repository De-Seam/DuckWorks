#pragma once
#include <Core/RTTI/RTTIRefObject.h>

// External includes
#include <External/entt/entity/registry.hpp>

class World;

class Entity : public RTTIRefObject
{
	RTTI_CLASS(Entity, RTTIRefObject)
public:
	virtual void Update(float inDeltaTime) { (void)inDeltaTime; }

	virtual void OnAddedToWorld(World* inWorld);
	virtual void OnRemovedFromWorld(World* inWorld);

	// Entity Component functions
	template<typename taType, typename... taArgs>
	taType& AddComponent(taArgs&&... inArgs);
	template<typename taType, typename... taArgs>
	taType& AddOrReplaceComponent(taArgs&&... inArgs);
	template<typename taType>
	taType& GetComponent();
	template<typename taType>
	bool HasComponent();
	template<typename taType>
	void RemoveComponent();

	entt::entity GetEntityHandle() const { return mEntityHandle; }

protected:
	entt::registry& GetRegistry();

private:
	entt::entity mEntityHandle = entt::null;
	World* mWorld = nullptr;
};

// Inline functions
template<typename taType, typename... taArgs>
taType& Entity::AddComponent(taArgs&&... inArgs)
{
	assert(!HasComponent<taType>()); //Entity already has component!
	taType& component = GetRegistry().emplace<taType>(mEntityHandle, std::forward<taArgs>(inArgs)...);
	return component;
}

template<typename taType, typename... taArgs>
taType& Entity::AddOrReplaceComponent(taArgs&&... inArgs)
{
	taType& component = GetRegistry().emplace_or_replace<taType>(mEntityHandle, std::forward<taArgs>(inArgs)...);
	return component;
}

template<typename taType>
taType& Entity::GetComponent()
{
	assert(HasComponent<taType>()); //Entity does not have component!
	return GetRegistry().get<taType>(mEntityHandle);
}

template<typename taType>
bool Entity::HasComponent()
{
	return GetRegistry().any_of<taType>(mEntityHandle);
}

template<typename taType>
void Entity::RemoveComponent()
{
	assert(HasComponent<taType>()); //Entity does not have component!
	GetRegistry().remove<taType>(mEntityHandle);
}