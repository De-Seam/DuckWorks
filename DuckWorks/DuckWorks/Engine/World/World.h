#pragma once
// Core includes
#include <Core/RTTI/RTTI.h>
#include <Core/RTTI/RTTIRefObject.h>

// Engine includes
#include <Engine/Entity/Entity.h>

// External includes
#include <External/entt/entity/registry.hpp>

class Entity;
class Grid;

class World : public RTTIClass
{
	RTTI_CLASS(World, RTTIClass)

public:
	World();
	virtual ~World() override;

	void Update(float inDeltaTime);
	void Render();

	void AddEntity(const Ref<Entity>& inEntity);
	void RemoveEntity(Entity& inEntity);

	Grid& GetGrid() { return *mGrid.get(); }
	const Grid& GetGrid() const { return *mGrid.get(); }

	entt::registry& GetRegistry() { return mRegistry; }
	const entt::registry& GetRegistry() const { return mRegistry; }

protected:
	void OnTransformComponentCreated(entt::registry& inRegistry, entt::entity inEntityHandle);

private:
	bool mIsUpdatingEntities = false;

	Array<Ref<Entity>> mEntities;

	UniquePtr<Grid> mGrid;
	entt::registry mRegistry;
};