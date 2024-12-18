#pragma once
// Game includes
#include <Game/Entity/Systems/EntitySystem.h>

class PlayerMovementSystem : public EntitySystemTick
{
	RTTI_CLASS(PlayerMovementSystem, EntitySystemTick)
public:
	PlayerMovementSystem(World& inWorld) : Base(inWorld) {}

private:
	virtual void Tick(float inDeltaTime) override;
};