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

	entt::registry& GetRegistry();

private:
	World* mWorld = nullptr;
};
