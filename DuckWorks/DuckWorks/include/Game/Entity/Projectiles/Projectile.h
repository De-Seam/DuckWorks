#pragma once
#include "Engine/Entity/Actor.h"

/*
Projectile provides base functionality for all projectiles
*/
class Projectile : public Actor
{
	RTTI_CLASS(Projectile, Actor)

public:
	Projectile();

	virtual void Update(float inDeltaTime) override;

	void SetVelocity(Vec2 inVelocity) { mVelocity = inVelocity; }

private:
	Vec2 mVelocity;
};
