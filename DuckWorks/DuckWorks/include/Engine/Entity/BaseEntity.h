#pragma once
// Engine includes
#include "Components.h"

// External includes
#include "Core/Utilities/UID.h"

#include "External/entt/entt.hpp"

class World;

class BaseEntity : public RTTIBaseClass
{
	RTTI_CLASS(BaseEntity, RTTIBaseClass)

public:
	BaseEntity(World* inWorld); ///< Will create a new handle with the world
	BaseEntity(entt::entity inHandle, World* inWorld); ///< Will assign the given handle to the entity
	BaseEntity(const BaseEntity& inOther) = default;

	~BaseEntity();

	virtual Json Serialize();
	virtual void Deserialize(const Json& inJson);

	void GenerateNewEntityHandle(World* inWorld);
	///< Helper function to generate a new entity handle for this entity. Should only be used for a child class which does not call the default constructor

	template<typename taType, typename... taArgs>
	taType& TryAddComponent(taArgs&&... inArgs);

	template<typename taType, typename... taArgs>
	taType& AddComponent(taArgs&&... inArgs);

	template<typename taType>
	decltype(auto) AddComponent()
	{
		assert(!HasComponent<taType>()); //Entity already has component!
		return GetRegistry().emplace<taType>(mEntityHandle);
	}

	template<typename taType, typename... taArgs>
	taType& AddOrReplaceComponent(taArgs&&... inArgs);

	template<typename taType>
	taType& GetComponent();

	template<typename taType>
	bool HasComponent();

	template<typename taType>
	void RemoveComponent();

	operator bool() const { return mEntityHandle != entt::null; }
	operator entt::entity() const { return mEntityHandle; }
	operator uint32_t() const { return static_cast<uint32_t>(mEntityHandle); }

	const std::string& GetName() { return GetComponent<NameComponent>().mName; }
	World* GetWorld() const { return mWorld; }

	bool operator==(const BaseEntity& inOther) const { return mEntityHandle == inOther.mEntityHandle && mWorld == inOther.mWorld; }
	bool operator!=(const BaseEntity& inOther) const { return !(*this == inOther); }

	entt::registry& GetRegistry() const;
	entt::entity GetEntityHandle() const { return mEntityHandle; }

protected:

protected:
	bool mHandleGotGenerated = false;
	entt::entity mEntityHandle = {entt::null};
	World* mWorld = nullptr;

	friend class World;
};


template<typename taType, typename... taArgs>
taType& BaseEntity::TryAddComponent(taArgs&&... inArgs)
{
	if (HasComponent<taType>())
	{
		return GetComponent<taType>();
	}
	taType& component = GetRegistry().emplace<taType>(mEntityHandle, std::forward<taArgs>(inArgs)...);
	return component;
}

// Inline functions
template<typename taType, typename... taArgs>
taType& BaseEntity::AddComponent(taArgs&&... inArgs)
{
	assert(!HasComponent<taType>()); //Entity already has component!
	taType& component = GetRegistry().emplace<taType>(mEntityHandle, std::forward<taArgs>(inArgs)...);
	return component;
}

//template<typename taType>
//taType& BaseEntity::AddComponent()
//{
//	assert(!HasComponent<taType>()); //Entity already has component!
//	taType& component = GetRegistry().emplace<taType>(mEntityHandle);
//	return component;
//}

template<typename taType, typename... taArgs>
taType& BaseEntity::AddOrReplaceComponent(taArgs&&... inArgs)
{
	taType& component = GetRegistry().emplace_or_replace<taType>(mEntityHandle, std::forward<taArgs>(inArgs)...);
	return component;
}

template<typename taType>
taType& BaseEntity::GetComponent()
{
	assert(HasComponent<taType>()); //Entity does not have component!
	return GetRegistry().get<taType>(mEntityHandle);
}

template<typename taType>
bool BaseEntity::HasComponent()
{
	return GetRegistry().any_of<taType>(mEntityHandle);
}

template<typename taType>
void BaseEntity::RemoveComponent()
{
	assert(HasComponent<taType>()); //Entity does not have component!
	GetRegistry().remove<taType>(mEntityHandle);
}
