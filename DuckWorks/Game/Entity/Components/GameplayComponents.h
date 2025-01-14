#pragma once
// Core includes
#include <DuckCore/Math/Transform.h>
#include <Game/Entity/Components/Component.h>

struct TransformComponent : public ComponentBase
{
	RTTI_CLASS(TransformComponent, ComponentBase)
public:
	DC::Transform2D mTransform;

	virtual void UpdateImGui() override;
};

struct VelocityComponent : public ComponentBase
{
	RTTI_CLASS(VelocityComponent, ComponentBase)
public:
	DC::FVec2 mVelocity = {0.0f, 0.0f};
};

struct MovementComponent : public ComponentBase
{
	RTTI_CLASS(MovementComponent, ComponentBase)
public:
	float mMaxVelocity = 100.0f;
	float mAcceleration = 80.0f;
};

struct BlockingComponent : public ComponentBase
{
	RTTI_CLASS(BlockingComponent, ComponentBase)
public:
	enum class BlockingType : uint8
	{
		None, // None. Other entities may walk over this.
		Full, // Full. Other entities may not walk over this.
		Half, // Half. Movement speed is lowered while "climbing" over this.
		Pawn // Pawn. Other entities may walk over this, but not stand on it.
	};

	BlockingType mBlockingType = BlockingType::None;
};