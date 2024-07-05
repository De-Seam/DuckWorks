#pragma once
// Engine includes
#include <Engine/Entity/Actor.h>

class MovingPlatform : public Actor
{
	RTTI_CLASS(MovingPlatform, Actor, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters
	{
		Vec2 mMoveExtents = Vec2(0.0f);
		Vec2 mMoveSpeed = Vec2(0.0f);
		Vec2 mStartPosition = Vec2(0.0f);
	};

	MovingPlatform(const ConstructParameters& inConstructParameters);

	virtual void BeginPlay() override;
	virtual void Update(float inDeltaTime) override;

private:
	Vec2 mMoveExtents;
	Vec2 mMoveSpeed;
	Vec2 mStartPosition;

private:
};
