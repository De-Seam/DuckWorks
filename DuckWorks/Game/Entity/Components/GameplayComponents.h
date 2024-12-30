#pragma once
// Core includes
#include <DuckCore/Math/Transform.h>

struct TransformComponent
{
	DC::Transform2D mTransform;
};

struct VelocityComponent
{
	DC::FVec2 mVelocity = {0.0f, 0.0f};
};

struct PlayerMovementComponent
{
	float mMaxVelocity = 100.0f;
	float mAcceleration = 80.0f;
};