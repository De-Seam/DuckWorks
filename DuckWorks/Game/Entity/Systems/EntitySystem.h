#pragma once
#include <Engine/Objects/Object.h>

#include <External/entt/fwd.hpp>

class World;

// Entity system base class
class EntitySystem : public Object
{
	RTTI_CLASS(EntitySystem, Object)
public:

	World& GetWorld() { return *mWorld; }
	const World& GetWorld() const { return *mWorld; }

	entt::registry& GetRegistry();

private:
	EntitySystem(World& inWorld) : mWorld(&inWorld) {}

	World* mWorld = nullptr;

	friend class EntitySystemUpdate;
	friend class EntitySystemTick;
};

// Entity System that updates every frame
class EntitySystemUpdate : public EntitySystem
{
	RTTI_CLASS(EntitySystemUpdate, EntitySystem)
public:
	EntitySystemUpdate(World& inWorld) : EntitySystem(inWorld) {}

	virtual void Update(float inDeltaTime) = 0;
};

// Entity System that updates every tick
class EntitySystemTick : public EntitySystem
{
	RTTI_CLASS(EntitySystemTick, EntitySystem)
public:
	EntitySystemTick(World& inWorld) : EntitySystem(inWorld) {}

	virtual void Tick(float inDeltaTime) = 0;
};
