#pragma once
// Engine includes
#include <Engine/Objects/Object.h>

class World;

class EntitySystem : public Object
{
	RTTI_CLASS(EntitySystem, Object)
public:
	EntitySystem(World& inWorld) : mWorld(&inWorld) {}

	World* GetWorld() { return mWorld; }
	const World* GetWorld() const { return mWorld; }

private:
	World* mWorld = nullptr;
};
