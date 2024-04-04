#pragma once
// Engine includes
#include <Engine/Entity/CollisionActor.h>

class MovingPlatform : public CollisionActor
{
	RTTI_CLASS(MovingPlatform, CollisionActor)

public:
	MovingPlatform() = default;
	virtual void Init(const Entity::InitParams& inInitParams) override;

	virtual void BeginPlay() override;
	virtual void Update(float inDeltaTime) override;

private:

	fm::vec2 mMoveExtents;
	fm::vec2 mMoveSpeed;
	fm::vec2 mStartPosition;

private:
};
