#pragma once
// Core includes
#include <Core/Math/Transform.h>

// Engine includes
#include <Engine/Entity/Components/EntityComponent.h>

// External includes
#include <External/entt/fwd.hpp>

class TransformComponent : public EntityComponent
{
	RTTI_CLASS(TransformComponent, EntityComponent)

public:
	// Set the Transform. This will also give the Entity the TransformUpdatedTag
	void SetTransform(const Transform2D& inTransform, entt::registry& inRegistry, entt::entity inEntityHandle);
	const Transform2D& GetTransform() const { return mTransform; }

private:
	Transform2D mTransform;
};

class TransformUpdatedTag : public EntityComponentTag
{
	RTTI_CLASS(TransformUpdatedTag, EntityComponentTag)
};

