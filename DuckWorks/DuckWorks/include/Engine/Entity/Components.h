#pragma once
// Core includes
#include "Core/Utilities/Utilities.h"
#include "Core/Math/FMath.h"

// Engine includes
#include "Engine/Resources/ResourceTypes/TextureResource.h"

class b2Body;

struct NameComponent
{
	String mName;
};

// Texture render component uses TransformComponent for its transform
struct TextureRenderComponent
{
	std::shared_ptr<TextureResource> mTexture;
	fm::vec4 mTintColor = {1.f, 1.f, 1.f, 1.f};
};

struct PhysicsComponent
{
	b2Body* mBody = nullptr;
};

struct TransformComponent
{
	fm::Transform2D mTransform = {};
};


// Tag components

// This tag is used to mark entities that have been teleported and need to update their physics position.
// They will automatically get cleared after usage
struct PhysicsPositionUpdatedTag
{};
