#pragma once
// Core includes
#include <Core/Math/Transform.h>

// Engine includes
#include <Engine/Entity/Components/EntityComponent.h>

// Component with a local offset transform in world space
class WorldComponent : public EntityComponent
{
	RTTI_CLASS(WorldComponent, EntityComponent)

public:
	Transform2D CalculateWorldTransform(const Transform2D& inParentTransform) const
	{
		return inParentTransform * mOffset;
	}

	Transform2D mOffset;
};