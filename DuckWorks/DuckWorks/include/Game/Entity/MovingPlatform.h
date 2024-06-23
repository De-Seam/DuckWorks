#pragma once
// Engine includes
#include <Engine/Entity/Actor.h>

class MovingPlatform : public Actor
{
	RTTI_CLASS(MovingPlatform, Actor, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters
	{
		fm::vec2 mMoveExtents = fm::vec2(0.0f);
		fm::vec2 mMoveSpeed = fm::vec2(0.0f);
		fm::vec2 mStartPosition = fm::vec2(0.0f);
	};

	MovingPlatform(const ConstructParameters& inConstructParameters);

	virtual void BeginPlay() override;
	virtual void Update(float inDeltaTime) override;

private:
	fm::vec2 mMoveExtents;
	fm::vec2 mMoveSpeed;
	fm::vec2 mStartPosition;

private:
};
