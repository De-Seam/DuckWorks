#pragma once
// Engine includes
#include <Engine/Entity/Actor.h>

class MovingPlatform : public Actor
{
	RTTI_CLASS(MovingPlatform, Actor)

public:
	MovingPlatform(World* inWorld);

	virtual void BeginPlay() override;
	virtual void Update(float inDeltaTime) override;

private:

	fm::vec2 mMoveExtents;
	fm::vec2 mMoveSpeed;
	fm::vec2 mStartPosition;

private:
};
