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
		Vec2 mVelocity = Vec2(0.0f);
	};

	Projectile(const ConstructParameters& inConstructParameters);

	virtual void Update(float inDeltaTime) override;

	void SetVelocity(Vec2 inVelocity) { mVelocity = inVelocity; }

private:
	Vec2 mVelocity;
};
