#pragma once
// Game includes
#include <Game/Entity/Systems/EntitySystem.h>

class RenderSystem : public EntitySystemUpdate
{
	RTTI_CLASS(RenderSystem, EntitySystemUpdate)
public:
	RenderSystem(World& inWorld) : Base(inWorld) {}

private:
	virtual void Update(float inDeltaTime) override;
};