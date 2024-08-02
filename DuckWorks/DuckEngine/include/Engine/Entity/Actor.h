#pragma once
#include <Engine/Entity/Entity.h>

class Actor : public Entity
{
	RTTI_CLASS(Actor, Entity)

public:
	virtual void OnAddedToWorld(World* inWorld);
};
