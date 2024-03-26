#pragma once
#include "Core/CoreBase.h"

#include "Actor.h"

// Simple actor with collision enabled by default.
// It's transform setters are overwritten with collision in mind
class CollisionActor : public Actor
{
	RTTI_CLASS(CollisionActor, Actor)
public:
	CollisionActor(World* inWorld);

	virtual void SetTransform(const fm::Transform2D& inTransform) override;
	virtual void SetPosition(const fm::vec2& inPosition) override;
	virtual void SetHalfSize(const fm::vec2& inHalfSize) override;
	virtual void SetRotation(float inRotation) override;



};