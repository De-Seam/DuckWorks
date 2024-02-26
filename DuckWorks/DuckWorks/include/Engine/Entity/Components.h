#pragma once
// Core includes
#include "Core/Utilities/Utilities.h"
#include "Core/Math/FMath.h"

// Engine includes
#include "Engine/Resources/ResourceTypes/TextureResource.h"
#include "Engine/Renderer/AnimationBase.h"

class b2Body;

struct NameComponent
{
	String mName;
};

// Texture render component uses TransformComponent for its transform
struct TextureRenderComponent
{
	std::shared_ptr<TextureResource> mTexture;
	fm::ivec4 mSrcRect = {};
	bool mUseSrcRect = false;
};

// Animation component pairs with TextureRenderComponent
struct AnimationComponent
{
	std::shared_ptr<AnimationBase> mAnimation = nullptr;

	// Private variables
	AnimationBase::Frame mCurrentFrame = {};
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
