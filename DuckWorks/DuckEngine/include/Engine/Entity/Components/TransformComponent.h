#pragma once
// Core includes
#include <Core/Math/Transform.h>

// Engine includes
#include <Engine/Entity/Components/EntityComponent.h>

class TransformComponent : public EntityComponent
{
	RTTI_CLASS(TransformComponent, EntityComponent)

public:
	Transform2D mTransform;
};