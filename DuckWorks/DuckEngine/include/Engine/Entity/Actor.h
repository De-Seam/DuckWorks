#pragma once
#include <Engine/Entity/Entity.h>

class Actor : public Entity
{
	RTTI_CLASS(Actor, Entity)

public:
	virtual void Update(float inDeltaTime) override;

	virtual void OnAddedToWorld(World* inWorld) override;
};
