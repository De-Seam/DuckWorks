#pragma once
// Core includes
#include "Core/CoreBase.h"
#include "Core/RTTI/RefObject.h"
#include "Core/Utilities/UID.h"
#include "Core/Utilities/Utilities.h"

// Engine includes
#include "Engine/Entity/Components/EntityComponent.h"

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
	virtual void Update([[maybe_unused]] float inDeltaTime) {}

	void Destroy();

	template<typename taType>
	taType* AddComponent(const typename taType::ConstructParameters& inConstructParameters = {});
	void AddComponent(EntityComponent* inComponent);
	template<typename taType>
	void GetComponentsOfType(Array<taType*>& outComponents);
	Array<EntityComponent*> GetComponentsOfType(UID inComponentUID);
	int32 GetComponentCountOfType(UID inComponentUID);
	template<typename taType>
	taType* GetFirstComponentOfType();
	template<typename taType>
	bool HasComponent();
	bool HasComponent(UID inComponentUID);
	void RemoveComponent(EntityComponent* inEntityComponent);

	template<typename taType>
	void LoopOverComponents(const Function<void(taType& inComponent)>& inFunction);
	void LoopOverComponents(UID inComponentUID, const Function<void(EntityComponent& inComponent)>& inFunction);

	void LoopOverAllComponents(const Function<void(EntityComponent& inComponent)>& inFunction);

	virtual void SetPosition(const Vec2& inPosition);
	virtual void SetRotation(float inRotation);

	[[nodiscard]] Vec2 GetPosition() const;
	[[nodiscard]] float GetRotation() const;

	[[nodiscard]] World* GetWorld() { return mWorld; }
	[[nodiscard]] const World* GetWorld() const { return mWorld; }
	[[nodiscard]] const String& GetName() const { return mName; }

private:
	Vec2 mPosition = {};
	float mRotation = 0.0f;

	// HashMap of [Component UID] to Array of Entity Components
	HashMap<UID, Array<Ref<EntityComponent>>> mEntityComponents;

	String mName;
	World* mWorld = nullptr;

	int32 mEntityComponentSalt = 0;
};

template<typename taType>
taType* Entity::AddComponent(const typename taType::ConstructParameters& inConstructParameters)
{
	typename taType::ConstructParameters parameters = inConstructParameters;

	parameters.mEntity = this;
	taType* component = taType::sNewInstance(parameters);

	AddComponent(component);

	return component;
}

template<typename taType>
void Entity::GetComponentsOfType(Array<taType*>& outComponents)
{
	Array<EntityComponent*>& components = mEntityComponents[taType::sGetRTTIUID()];
	for (EntityComponent* component : components)
		outComponents.emplace_back(SCast<taType*>(component));
}

template<typename taType>
taType* Entity::GetFirstComponentOfType()
{
	if (mEntityComponents.empty())
		return nullptr;
	return SCast<taType*>(mEntityComponents[taType::sGetRTTIUID()].front().Get());
}

template<typename taType>
bool Entity::HasComponent()
{
	auto iterator = mEntityComponents.find(taType::sGetRTTIUID());
	return iterator != mEntityComponents.end() && !iterator->second.empty();
}

template<typename taType>
void Entity::LoopOverComponents(const Function<void(taType& inComponent)>& inFunction)
{
	Array<Ref<EntityComponent>>& components = mEntityComponents[taType::sGetRTTIUID()];
	for (EntityComponent* component : components)
		inFunction(*SCast<taType*>(component));
}
