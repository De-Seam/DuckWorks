#pragma once
#include "Core/CoreBase.h"

#include "Actor.h"

// Simple actor with collision enabled by default.
// It's transform setters are overwritten with collision in mind
class CollisionActor : public Actor
{
	RTTI_CLASS(CollisionActor, Actor)
public:
	CollisionActor();
	virtual ~CollisionActor() override;
	virtual void Init(const Entity::InitParams& inInitParams) override;

	virtual void BeginPlay() override;

	// The variables you pass here are the RENDER variables. The Collision variables are adjusted using mRelativeTransform
	virtual fm::Transform2D MoveTo(Optional<fm::vec2> inPosition, Optional<float> inRotation = NullOpt, Optional<fm::vec2> inHalfSize = NullOpt);
	void TeleportPosition(const fm::vec2& inPosition);
	void TeleportTransform(const fm::Transform2D& inTransform);
};