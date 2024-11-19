#pragma once
// Game includes
#include <Game/Entity/Systems/EntitySystem.h>

class WorldUpdateHandle;

class RenderSystem : public EntitySystemUpdate
{
	RTTI_CLASS(RenderSystem, EntitySystem)
public:
	RenderSystem(World& inWorld) : EntitySystemUpdate(inWorld) {}

private:
	virtual void Update(float inDeltaTime) override;

	DC::Ref<WorldUpdateHandle> mUpdateHandle;
};