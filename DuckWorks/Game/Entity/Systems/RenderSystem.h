#pragma once
// Game includes
#include <Game/Entity/Systems/EntitySystem.h>

class WorldUpdateHandle;

class RenderSystem : public EntitySystem
{
	RTTI_CLASS(RenderSystem, EntitySystem)
public:
	RenderSystem(World& inWorld);

private:
	void Update(float inDeltaTime);

	DC::Ref<WorldUpdateHandle> mUpdateHandle;
};