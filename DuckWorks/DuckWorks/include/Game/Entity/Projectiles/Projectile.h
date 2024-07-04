#pragma once
#include "Engine/Entity/Actor.h"

/*
Projectile provides base functionality for all projectiles
*/
class Projectile : public Actor
{
	RTTI_CLASS(Projectile, Actor, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters
	{
		fm::vec2 mVelocity = fm::vec2(0.0f);
	};

	Projectile(const ConstructParameters& inConstructParameters);

	virtual void Update(float inDeltaTime) override;

	void SetVelocity(fm::vec2 inVelocity) { mVelocity = inVelocity; }

private:
	fm::vec2 mVelocity;
};
