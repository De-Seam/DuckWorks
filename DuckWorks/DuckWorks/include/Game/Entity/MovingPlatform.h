#pragma once
// Engine includes
#include <Engine/Entity/CollisionActor.h>

class MovingPlatform : public CollisionActor
{
	RTTI_CLASS(MovingPlatform, CollisionActor, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters
	{
		fm::vec2 mMoveExtents = fm::vec2(0.0f);
		fm::vec2 mMoveSpeed = fm::vec2(0.0f);
		fm::vec2 mStartPosition = fm::vec2(0.0f);
	};

	MovingPlatform(const ConstructParameters& inConstructParameters);
	virtual void Init(const InitParams& inInitParams) override;

	virtual void BeginPlay() override;
	virtual void Update(float inDeltaTime) override;

private:
	fm::vec2 mMoveExtents;
	fm::vec2 mMoveSpeed;
	fm::vec2 mStartPosition;

private:
};
