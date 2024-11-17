#pragma once
// Engine includes
#include <Engine/Objects/Object.h>

// External includes
#include <External/entt/entt.hpp>

class World;

class Entity : public Object
{
	RTTI_CLASS(Entity, Object)
public:
	Entity(World* inWorld);
	virtual ~Entity() override;

	template<typename TaType, typename... taArgs>
	TaType& AddComponent(taArgs&&... inArgs);
	template<typename taType, typename... taArgs>
	taType& AddOrReplaceComponent(taArgs&&... inArgs);

	template<typename taType>
	bool HasComponent();
	template<typename taType>
	void RemoveComponent();

	template<typename taType>
	taType& GetComponent();
	template<typename taType>
	const taType& GetComponent() const;
	template<typename taType>
	taType* FindComponent();
	template<typename taType>
	const taType* FindComponent() const;

	World* GetWorld() { return mWorld; }
	const World* GetWorld() const { return mWorld; }

	entt::entity GetEntityHandle() { return mEntityHandle; } // Non-const because the handle allows for modification
	entt::registry& GetRegistry();
	const entt::registry& GetRegistry() const;

private:
	World* mWorld = nullptr;
	entt::entity mEntityHandle = entt::null;
};

template<typename taType, typename... taArgs>
taType& Entity::AddComponent(taArgs&&... inArgs)
{
	gAssert(!HasComponent<taType>()); //Entity already has component!
	taType& component = GetRegistry().emplace<taType>(mEntityHandle, std::forward<taArgs>(inArgs)...);
	return component;
}

template<typename taType, typename... Args>
taType& Entity::AddOrReplaceComponent(Args&&... inArgs)
{
	taType& component = GetRegistry().emplace_or_replace<taType>(mEntityHandle, std::forward<Args>(inArgs)...);
	return component;
}

template<typename taType>
bool Entity::HasComponent()
{
	return GetRegistry().any_of<taType>(mEntityHandle);
}

template<typename taType>
void Entity::RemoveComponent()
{
	gAssert(HasComponent<taType>()); //Entity does not have component!
	GetRegistry().remove<taType>(mEntityHandle);
}

template<typename taType>
taType& Entity::GetComponent()
{
	gAssert(HasComponent<taType>()); //Entity does not have component!
	return GetRegistry().get<taType>(mEntityHandle);
}

template<typename taType>
const taType& Entity::GetComponent() const
{
	gAssert(HasComponent<taType>()); //Entity does not have component!
	return GetRegistry().get<taType>(mEntityHandle);
}

template<typename taType>
taType* Entity::FindComponent()
{
	return GetRegistry().try_get<taType>(mEntityHandle);
}

template<typename taType>
const taType* Entity::FindComponent() const
{
	return GetRegistry().try_get<taType>(mEntityHandle);
}
