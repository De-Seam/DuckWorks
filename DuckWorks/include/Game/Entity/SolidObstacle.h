#pragma once
// Engine includes
#include "Engine/Entity/Actor.h"

class SolidObstacle : public Actor
{
	RTTI_CLASS(SolidObstacle, Actor, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	SolidObstacle(const ConstructParameters& inConstructParameters);
	virtual void Init() override;
};
