#pragma once
#include "Entity.h"

// An actor is an entity that has a transform component. It can be moved and rotated
class Actor : public Entity
{
	RTTI_CLASS(Actor, Entity)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	using Base::Base;

	Actor(const ConstructParameters& inConstructParameters = {}) : Base(inConstructParameters) {}
};
