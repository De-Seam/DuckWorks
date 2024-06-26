#pragma once
#include "Core/CoreBase.h"

// Engine includes
#include "Engine/Entity/Actor.h"

class EnemyBase : public Actor
{
	RTTI_CLASS(EnemyBase, Actor)
public:
	EnemyBase() = default;
	virtual void Init(const Entity::InitParams& inInitParams) override;

	void Update(float inDeltaTime);

private:
	EntityWeakPtr mPlayer;
};