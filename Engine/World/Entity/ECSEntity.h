#pragma once
#include <entt/entity/fwd.hpp>
#include <entt/entity/registry.hpp>

class Scene;

// A helper class wrapping an ECS entity.
class ECSEntity
{
public:
	explicit ECSEntity(Scene* aScene);

	template<typename taType, typename... taArgs>
	taType& AddComponent(taArgs&& aArguments);
	template<typename taType, typename... taArgs>
	taType& AddOrReplaceComponent(taArgs&& aArguments);

	template<typename taType>
	void RemoveComponent();
	template<typename taType>
	void TryRemoveComponent();

	template<typename taType>
	taType& GetComponent();
	template<typename taType>
	const taType& GetComponent() const;

	template<typename taType>
	taType* FindComponent();
	template<typename taType>
	const taType* FindComponent() const;

	template<typename  taType>
	bool HasComponent() const;

	Scene* GetScene() { return mScene; }
	const Scene* GetScene() const { return mScene; }

	entt::entity GetEntityHandle() { return mEntityHandle; }

	entt::registry& GetRegistry();
	const entt::registry& GetRegistry() const;

protected:
	ECSEntity() = default;

	Scene* mScene = nullptr; // The scene this Entity is in.

	entt::entity mEntityHandle = entt::null; // The EntityHandle in the ECS.
};

template<typename taType, typename... taArgs>
taType& ECSEntity::AddComponent(taArgs&& aArguments)
{
	entt::registry& registry = GetRegistry();
	return registry.emplace<taType>(std::forward<taArgs>(aArguments)...);
}

template<typename taType, typename ... taArgs>
taType& ECSEntity::AddOrReplaceComponent(taArgs&& aArguments)
{
	entt::registry& registry = GetRegistry();
	return registry.emplace_or_replace<taType>(std::forward<taArgs>(aArguments)...);
}

template<typename taType>
void ECSEntity::RemoveComponent()
{
	gAssert(HasComponent<taType>());
	GetRegistry().remove<taType>(mEntityHandle);
}

template<typename taType>
void ECSEntity::TryRemoveComponent()
{
	if (HasComponent<taType>())
		RemoveComponent<taType>();
}

template<typename taType>
taType& ECSEntity::GetComponent()
{
	gAssert(HasComponent<taType>());
	return GetRegistry().get<taType>();
}

template<typename taType>
const taType& ECSEntity::GetComponent() const
{
	gAssert(HasComponent<taType>());
	return GetRegistry().get<taType>();
}

template<typename taType>
taType* ECSEntity::FindComponent()
{
	return HasComponent<taType>() ? &GetComponent<taType>() : nullptr;
}

template<typename taType>
const taType* ECSEntity::FindComponent() const
{
	return HasComponent<taType>() ? &GetComponent<taType>() : nullptr;
}

template<typename taType>
bool ECSEntity::HasComponent() const
{
	return GetRegistry().any_of<taType>();
}
