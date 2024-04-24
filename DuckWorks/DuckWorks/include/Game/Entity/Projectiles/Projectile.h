#pragma once
#include "Core/CoreBase.h"

#include "Engine/Entity/CollisionActor.h"

/*
Projectile provides base functionality for all projectiles
*/
class Projectile : public CollisionActor
{
	RTTI_CLASS(Projectile, CollisionActor)
public:
	Projectile();

	virtual void Init(const InitParams &inInitParams) override;

	virtual void Update(float inDeltaTime) override;

	void SetVelocity(fm::vec2 inVelocity) { mVelocity = inVelocity; }

private:
	fm::vec2 mVelocity;
};