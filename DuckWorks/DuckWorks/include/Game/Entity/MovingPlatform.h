#pragma once
// Engine includes
#include <Engine/Entity/Actor.h>

class MovingPlatform : public Actor
{
	RTTI_CLASS(MovingPlatform, Actor)

public:
	virtual Json Serialize() const override;
	virtual void Deserialize(const Json& inJson) override;

	MovingPlatform();

	virtual void BeginPlay() override;
	virtual void Update(float inDeltaTime) override;

private:
	Vec2 mMoveExtents;
	Vec2 mMoveSpeed;
	Vec2 mStartPosition;

private:
};
