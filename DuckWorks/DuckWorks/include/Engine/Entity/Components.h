#pragma once
// Core includes
#include "Core/Utilities/Utilities.h"
#include "Core/Math/FMath.h"

// Engine includes
#include "Engine/Resources/ResourceTypes/TextureResource.h"
#include "Engine/Renderer/Animation.h"

class b2Body;

struct NameComponent
{
	String mName;
};

// Texture render component uses TransformComponent for its transform
struct TextureRenderComponent
{
	bool mUseSrcRect = false;
	fm::ivec4 mSrcRect = {};
	std::shared_ptr<TextureResource> mTexture;
	fm::vec4 mTintColor = {1.f, 1.f, 1.f, 1.f};
};

// Animation component pairs with TextureRenderComponent
struct AnimationComponent
{
	std::shared_ptr<Animation> mAnimation = nullptr;

	// Private variables
	Animation::Frame mCurrentFrame = {};
	float mTimeSinceUpdate = 0.f;
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
