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

struct BlockingComponent
{
	enum class BlockingType : uint8
	{
		None, // None. Other entities may walk over this.
		Full, // Full. Other entities may not walk over this.
		Half, // Half. Movement speed is lowered while "climbing" over this.
		Pawn // Pawn. Other entities may walk over this, but not stand on it.
	};

	BlockingType mBlockingType = BlockingType::None;
};