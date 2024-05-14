#pragma once
// Core includes
#include "Core/CoreBase.h"
#include "Core/Utilities/RefObject.h"
#include "Core/Utilities/UID.h"
#include "Core/Utilities/Utilities.h"

// Engine includes
#include "Engine/Entity/Components/EntityComponent.h"
#include "Engine/Entity/Components/EntityComponentManager.h"

/*
 * Entity rules:
 * An EntityPtr (SharedPtr<Entity>) is a reference to an entity which is always valid.
 * It is possible for an EntityPtr to reference an entity that has been destroyed, but the EntityPtr will still be valid.
 * An EntityWeakPtr (WeakPtr<Entity>) is a reference to an entity which may be invalid.
 */
/*
Order of initialization:
1. Constructor. World and entity handle are not yet set. It's not possible to add components.
2. Init. This sets the world and the handle. Here you can initialize components. It's called in the construction phase.
3. BeginPlay. This is called on BeginPlay of the world.
*/

class World;

class Entity : public RefObject
{
	RTTI_CLASS(Entity, RefObject)

public:
	Entity() = default;
	virtual ~Entity() override;

	struct InitParams
	{
		World* mWorld = nullptr;
		String mName;
	};

	virtual void Init(const InitParams& inInitParams)
	{
		mWorld = inInitParams.mWorld;
		mName = inInitParams.mName;
	}

	virtual void BeginPlay() {}
	virtual void EndPlay() {}
	virtual void Update(float inDeltaTime) { (void)inDeltaTime; }

	void Destroy();

	template<typename taType, typename... taArgs>
	void AddComponent(taArgs&&... inArgs);
	template<typename taType>
	Array<MutexReadProtectedPtr<taType>> GetComponentsOfType(); ///< Warning: Slow!
	Array<MutexReadProtectedPtr<EntityComponent>> GetComponentsOfType(UID inComponentUID);
	template<typename taType>
	MutexReadProtectedPtr<taType> GetFirstComponentOfType();
	template<typename taType>
	bool HasComponent();
	bool HasComponent(UID inComponentUID);
	template<typename taType>
	void RemoveComponent(Handle<EntityComponent> inHandle);

	template<typename taType>
	void LoopOverComponents(Function<void(taType& inComponent)> inFunction);

	virtual void SetTransform(const fm::Transform2D& inTransform);
	virtual void SetPosition(const fm::vec2& inPosition);
	virtual void SetHalfSize(const fm::vec2& inHalfSize);
	virtual void SetRotation(float inRotation);

	[[nodiscard]] fm::Transform2D GetTransform();
	[[nodiscard]] fm::vec2 GetPosition();
	[[nodiscard]] fm::vec2 GetHalfSize();
	[[nodiscard]] float GetRotation();

	[[nodiscard]] World* GetWorld() { return mWorld; }
	[[nodiscard]] const World* GetWorld() const { return mWorld; }
	[[nodiscard]] String GetName() { return mName; }

private:
	fm::Transform2D mTransform = {};
	Mutex mTransformMutex;

	// HashMap of [Component UID] to Array of Entity Component Handles
	HashMap<UID, Array<Handle<EntityComponent>>> mEntityComponents;
	Mutex mEntityComponentsMutex;

	String mName;
	World* mWorld = nullptr;
};

template<typename taType, typename... taArgs>
void Entity::AddComponent(taArgs&&... inArgs)
{
	ScopedMutexWriteLock lock(mEntityComponentsMutex);

	mEntityComponents[taType::sGetRTTIUID()].emplace_back(gEntityComponentManager.AddComponent<taType>(std::forward<taArgs>(inArgs)...));
}

template<typename taType>
Array<MutexReadProtectedPtr<taType>> Entity::GetComponentsOfType()
{
	ScopedMutexReadLock lock(mEntityComponentsMutex);
	Array<MutexReadProtectedPtr<taType>> return_array;

	Array<Handle<EntityComponent>>& components = mEntityComponents[taType::sGetRTTIUID()];
	for (Handle<EntityComponent>& component : components)
	{
		return_array.emplace_back(gEntityComponentManager.GetComponent<taType>(component));
	}

	return return_array;
}

template<typename taType>
inline MutexReadProtectedPtr<taType> Entity::GetFirstComponentOfType()
{
	ScopedMutexReadLock lock(mEntityComponentsMutex);

	Array<Handle<EntityComponent>>& components = mEntityComponents[taType::sGetRTTIUID()];

	gAssert(!components.empty(), "Component not found!");
	return gEntityComponentManager.GetComponent<taType>(components.front());
}

template<typename taType>
bool Entity::HasComponent()
{
	ScopedMutexReadLock lock(mEntityComponentsMutex);

	HashMap<UID, Array<Handle<EntityComponent>>>::iterator iterator = mEntityComponents.find(taType::sGetRTTIUID());
	return iterator != mEntityComponents.end() && !iterator->second.empty();
}

template<typename taType>
void Entity::RemoveComponent(Handle<EntityComponent> inHandle)
{
	ScopedMutexWriteLock lock(mEntityComponentsMutex);

	gEntityComponentManager.RemoveComponent<taType>(inHandle);
}

template<typename taType>
void Entity::LoopOverComponents(Function<void(taType& inComponent)> inFunction)
{
	ScopedMutexReadLock lock(mEntityComponentsMutex);
	Array<Handle<EntityComponent>>& component_handles = mEntityComponents[taType::sGetRTTIUID()];
	for (Handle<EntityComponent>& component_handle : component_handles)
	{
		MutexReadProtectedPtr<taType> component = gEntityComponentManager.GetComponent<taType>(component_handle);
		inFunction(*component.Get());
	}
}
