#pragma once
// Engine includes
#include "Engine/Entity/CollisionActor.h"

class SolidObstacle : public CollisionActor
{
	RTTI_CLASS(SolidObstacle, CollisionActor, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	using Base::Base;

	SolidObstacle(const ConstructParameters& inConstructParameters);
	virtual void Init() override;
};
