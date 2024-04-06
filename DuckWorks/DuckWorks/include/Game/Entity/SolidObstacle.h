#pragma once
// Engine includes
#include "Engine/Entity/CollisionActor.h"

class SolidObstacle : public CollisionActor
{
	RTTI_CLASS(SolidObstacle, CollisionActor)

public:
	SolidObstacle() = default;
	virtual void Init(const Entity::InitParams& inInitParams) override;
};
