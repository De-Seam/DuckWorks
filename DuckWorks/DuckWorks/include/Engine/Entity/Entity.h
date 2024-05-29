#pragma once
// Core includes
#include "Core/CoreBase.h"
#include "Core/RTTI/RefObject.h"
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
1. Constructor. Here you should create Components
2. Deserialize. This is called before Init and after the Constructor
2. Init. It's called in the construction phase.
3. BeginPlay. This is called on BeginPlay of the world.
*/

class World;

class Entity : public RefObject
{
	RTTI_CLASS(Entity, RefObject, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters
	{
		World* mWorld = nullptr;
		String mName;
	};

	Entity(const ConstructParameters& inConstructParameters)
		: Base(inConstructParameters)
	{
		mWorld = inConstructParameters.mWorld;
		mName = inConstructParameters.mName;
	}

	virtual ~Entity() override;

	virtual void Init() {}

	virtual void BeginPlay() {}
	virtual void EndPlay() {}
	virtual void Update(float inDeltaTime) { (void)inDeltaTime; }

	void Destroy();

	template<typename taType>
	taType* AddComponent(const typename taType::ConstructParameters& inConstructParameters = {});
	template<typename taType>
	Array<taType*> GetComponentsOfType(); ///< Warning: Slow!
	Array<EntityComponent*> GetComponentsOfType(UID inComponentUID);
	int32 GetComponentCountOfType(UID inComponentUID);
	template<typename taType>
	bool HasComponent();
	bool HasComponent(UID inComponentUID);
	void RemoveComponent(EntityComponent* inEntityComponent);

	template<typename taType>
	void LoopOverComponents(const Function<void(taType& inComponent)>& inFunction);
	void LoopOverComponents(UID inComponentUID, const Function<void(EntityComponent& inComponent)>& inFunction);

	void LoopOverAllComponents(const Function<void(EntityComponent& inComponent)>& inFunction);

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
	[[nodiscard]] const String& GetName() { return mName; }

private:
	fm::Transform2D mTransform = {};
	Mutex mTransformMutex;

	// HashMap of [Component UID] to Array of Entity Components
	HashMap<UID, Array<EntityComponent*>> mEntityComponents;
	Mutex mEntityComponentsMutex;

	String mName;
	World* mWorld = nullptr;

	int32 mEntityComponentSalt = 0;
};

template<typename taType>
taType* Entity::AddComponent(const typename taType::ConstructParameters& inConstructParameters)
{
	static GUID sBaseComponentGUID = GUID("52af-b8bb-1b48-d338");
	const GUID guid = GUID::sCombine(GetGUID(), sBaseComponentGUID, mEntityComponentSalt++);

#ifdef _DEBUG
	// Check if the component is already added
	LoopOverAllComponents([&guid](const EntityComponent& inExistingComponent)
	{
		if (inExistingComponent.GetGUID() == guid)
		{
			gLog(ELogType::Error, "Component with the same uuid already exists on entity");
			gAssert(false, "Component with the same uuid already exists on entity");
		}
	});
#endif // _DEBUG

	typename taType::ConstructParameters parameters = inConstructParameters;

	parameters.mEntity = this;
	taType* component = taType::sNewInstance(parameters);
	component->SetGUID(guid);

	ScopedMutexWriteLock lock(mEntityComponentsMutex);
	mEntityComponents[taType::sGetRTTIUID()].emplace_back(component);
	return component;
}

template<typename taType>
Array<taType*> Entity::GetComponentsOfType()
{
	Array<taType*> return_array;

	ScopedMutexReadLock lock(mEntityComponentsMutex);
	Array<EntityComponent*>& components = mEntityComponents[taType::sGetRTTIUID()];
	for (EntityComponent* component : components)
		return_array.emplace_back(SCast<taType*>(component));

	return return_array;
}

template<typename taType>
bool Entity::HasComponent()
{
	ScopedMutexReadLock lock(mEntityComponentsMutex);

	auto iterator = mEntityComponents.find(taType::sGetRTTIUID());
	return iterator != mEntityComponents.end() && !iterator->second.empty();
}

template<typename taType>
void Entity::LoopOverComponents(const Function<void(taType& inComponent)>& inFunction)
{
	ScopedMutexReadLock lock(mEntityComponentsMutex);
	Array<EntityComponent*>& components = mEntityComponents[taType::sGetRTTIUID()];
	for (EntityComponent* component : components)
		inFunction(*SCast<taType*>(component));
}
