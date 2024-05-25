#pragma once
#include "Core/CoreBase.h"

// Engine includes
#include "Engine/Entity/Actor.h"

class EnemyBase : public Actor
{
	RTTI_CLASS(EnemyBase, Actor)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	using Base::Base;

	EnemyBase() = default;
	virtual void Init(const InitParams& inInitParams) override;

	virtual void Update(float inDeltaTime) override;

private:
	EntityWeakPtr mPlayer;
};
