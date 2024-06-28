#pragma once
#include "Core/CoreBase.h"

// Engine includes
#include "Engine/Entity/Actor.h"

class EnemyBase : public Actor
{
	RTTI_CLASS(EnemyBase, Actor, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	EnemyBase(const ConstructParameters& inConstructParameters);
	virtual void Init() override;

	virtual void Update(float inDeltaTime) override;

private:
	WeakRef<Entity> mPlayer = nullptr;
	float mMaxVelocity = 100.0f;
	float mAcceleration = 100.f; ///< Velocity increment per second
	float mVelocity = 0.0f;
};
