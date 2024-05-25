#pragma once
// Engine includes
#include "Engine/Entity/CollisionActor.h"

class SolidObstacle : public CollisionActor
{
	RTTI_CLASS(SolidObstacle, CollisionActor)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	using Base::Base;

	SolidObstacle(const ConstructParameters& inConstructParameters) : Base(inConstructParameters) {}
	virtual void Init(const InitParams& inInitParams) override;
};
