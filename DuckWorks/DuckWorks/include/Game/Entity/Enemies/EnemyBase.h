#pragma once
#include "Core/CoreBase.h"

// Engine includes
#include "Engine/Entity/Actor.h"

class EnemyBase : public Actor
{
	RTTI_CLASS(EnemyBase, Actor, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	using Base::Base;

	EnemyBase() = default;
	virtual void Init() override;

	virtual void Update(float inDeltaTime) override;

private:
	WeakRef<Entity> mPlayer;
};
