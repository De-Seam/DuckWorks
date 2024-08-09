#pragma once
// Core includes
#include <Core/RTTI/RTTI.h>
#include <Core/RTTI/RTTIRefObject.h>

// Engine includes
#include <Engine/Node/Node.h>

// External includes
#include <External/entt/entity/registry.hpp>

class Entity;

class World : public RTTIClass
{
	RTTI_CLASS(World, RTTIClass)

public:
	World();

	void Update(float inDeltaTime);
	void Render();

	void AddEntity(const Ref<Entity>& inEntity);
	void RemoveEntity(const Ref<Entity>& inEntity);

	entt::registry& GetRegistry() { return mRegistry; }

protected:
	void OnTransformComponentCreated(entt::registry& inRegistry, entt::entity inEntityHandle);

private:
	bool mIsUpdatingEntities = false;

	Array<Ref<Node>> mNodes;

	Array<Ref<Entity>> mEntities;
	Array<Ref<Entity>> mEntitiesToAdd;
	Array<Ref<Entity>> mEntitiesToRemove;

	entt::registry mRegistry;
};