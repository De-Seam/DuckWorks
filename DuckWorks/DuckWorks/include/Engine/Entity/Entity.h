#pragma once
#include "BaseEntity.h"

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

class EntityRefComponent;

class Entity : public BaseEntity
{
	RTTI_CLASS(Entity, BaseEntity)

public:
	Entity(World* inWorld)
		: BaseEntity(inWorld) {}

	Entity() = default;
	virtual ~Entity() override;

	struct InitParams
	{
		World* mWorld = nullptr;
	};
	virtual void Init(const InitParams& inInitParams)
	{
		if (mEntityHandle == entt::null)
			GenerateNewEntityHandle(inInitParams.mWorld);
	}

	virtual void BeginPlay() {}
	virtual void EndPlay() {}
	virtual void Update(float inDeltaTime) { (void)inDeltaTime; }

public:
	const UID mUID = {};

	Array<Handle<EntityRefComponent>> mComponents;
};
