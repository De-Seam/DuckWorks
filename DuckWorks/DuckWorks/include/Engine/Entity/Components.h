#pragma once
// Core includes
#include "Core/Math/FMath.h"
#include "Core/Utilities/UID.h"
#include "Core/Utilities/Utilities.h"

// Engine includes
#include "Engine/Collision/CollisionStructs.h"
#include "Engine/Entity/Components/EntityComponent.h"
#include "Engine/Renderer/AnimationBase.h"
#include "Engine/Resources/ResourceTypes/TextureResource.h"

class Camera;
class b2Body;
class Entity;

#pragma warning( push )
#pragma warning( disable : 4324) // Structure was padded due to alignment specifier

// Texture render component uses TransformComponent for its transform
struct TextureRenderComponent : public EntityComponent
{
	RTTI_CLASS(TextureRenderComponent, EntityComponent, ClassAllocator)
	struct ConstructParameters : public Base::ConstructParameters
	{
		SharedPtr<TextureResource> mTexture = nullptr;
		fm::ivec4 mSrcRect = {};
		bool mUseSrcRect = false;
		SDL_RendererFlip mFlip = SDL_FLIP_NONE;
	};

	TextureRenderComponent(const ConstructParameters& inConstructParameters = {});

	SharedPtr<TextureResource> mTexture;
	fm::ivec4 mSrcRect = {};
	bool mUseSrcRect = false;

	// Set by the animation component if it has one
	SDL_RendererFlip mFlip = SDL_FLIP_NONE;
};

// Animation component pairs with TextureRenderComponent
struct AnimationComponent : public EntityComponent
{
	RTTI_CLASS(AnimationComponent, EntityComponent, ClassAllocator)
	struct ConstructParameters : public Base::ConstructParameters
	{
		SharedPtr<AnimationBase> mAnimation = nullptr;
	};

	AnimationComponent(const ConstructParameters& inConstructParameters = {})
		: Base(inConstructParameters),
		mAnimation(inConstructParameters.mAnimation) {}

	virtual void Update(float inDeltaTime) override;

private:
	// RTTI variables
	SharedPtr<AnimationBase> mAnimation = nullptr;

	AnimationBase::Frame mCurrentFrame = {};
	float mTimeSinceUpdate = 0.f;
};

struct HealthComponent : public EntityComponent
{
	RTTI_CLASS(HealthComponent, EntityComponent, ClassAllocator)
	struct ConstructParameters : public Base::ConstructParameters
	{
		float mHealth = 100.f;
	};

	HealthComponent(const ConstructParameters& inConstructParameters = {})
		: Base(inConstructParameters),
		mHealth(inConstructParameters.mHealth) {}

	float mHealth = 100.f;
};

struct CameraComponent : public EntityComponent
{
	RTTI_CLASS(CameraComponent, EntityComponent, ClassAllocator)
	struct ConstructParameters : public Base::ConstructParameters
	{
		SharedPtr<Camera> mCamera = nullptr;
		int32 mPriority = 0; ///< Higher priority cameras will render on top of lower priority cameras
		bool mIsActive = false; ///< If the camera is active, it will render based on priority
	};

	CameraComponent(const ConstructParameters& inConstructParameters = {});

	SharedPtr<Camera> mCamera = nullptr;
	int32 mPriority = 0; ///< Higher priority cameras will render on top of lower priority cameras
	bool mIsActive = false; ///< If the camera is active, it will render based on priority
};
