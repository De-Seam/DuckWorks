#pragma once
// Core includes
#include "Core/Utilities/Utilities.h"
#include "Core/Math/FMath.h"

// Engine includes
#include "Core/Utilities/UID.h"

#include "Engine/Collision/CollisionStructs.h"
#include "Engine/Resources/ResourceTypes/TextureResource.h"
#include "Engine/Renderer/AnimationBase.h"

class Camera;
class b2Body;
class Entity;

#pragma warning( push )
#pragma warning( disable : 4324) // Structure was padded due to alignment specifier

struct ComponentBase : public RTTIBaseClass
{
	RTTI_CLASS(ComponentBase, RTTIBaseClass)

	// This is a base class for all components
	// It is used to allow for polymorphism in the component manager
	// It is not meant to be used directly
};

// EntityComponent is added to each entity to point towards the weak ptr entity
struct EntityComponent : public ComponentBase
{
	RTTI_CLASS(EntityComponent, ComponentBase)

	EntityComponent() = default;
	EntityComponent(WeakRef<Entity> mEntity) : mEntity(mEntity) {}

	WeakRef<Entity> mEntity;
};

struct NameComponent : public ComponentBase
{
	RTTI_CLASS(NameComponent, ComponentBase)

	NameComponent() = default;
	NameComponent(String inName) : mName(inName) {}

	String mName;
};

// Texture render component uses TransformComponent for its transform
struct TextureRenderComponent : public ComponentBase
{
	RTTI_CLASS(TextureRenderComponent, ComponentBase)

	TextureRenderComponent();

	TextureRenderComponent(
		SharedPtr<TextureResource> inTexture,
		fm::ivec4 inSrcRect,
		bool inUseSrcRect,
		SDL_RendererFlip inFlip
	)
		:
		mTexture(inTexture),
		mSrcRect(inSrcRect),
		mUseSrcRect(inUseSrcRect),
		mFlip(inFlip)
	{}

	SharedPtr<TextureResource> mTexture;
	fm::ivec4 mSrcRect = {};
	bool mUseSrcRect = false;

	// Set by the animation component if it has one
	SDL_RendererFlip mFlip = SDL_FLIP_NONE;
};

// Animation component pairs with TextureRenderComponent
struct AnimationComponent : public ComponentBase
{
	RTTI_CLASS(AnimationComponent, ComponentBase)

	AnimationComponent() = default;

	AnimationComponent(
		SharedPtr<AnimationBase> inAnimation,
		AnimationBase::Frame inCurrentFrame,
		float inTimeSinceUpdate
	)
		:
		mAnimation(inAnimation),
		mCurrentFrame(inCurrentFrame),
		mTimeSinceUpdate(inTimeSinceUpdate)
	{}

	SharedPtr<AnimationBase> mAnimation = nullptr;

	// Private variables
	AnimationBase::Frame mCurrentFrame = {};
	float mTimeSinceUpdate = 0.f;
};

struct CollisionComponent : public ComponentBase
{
	RTTI_CLASS(CollisionComponent, ComponentBase)

	CollisionComponent();
	CollisionComponent(const CollisionObjectHandle& inCollisionObjectHandle) : mCollisionObjectHandle(inCollisionObjectHandle) {}
	~CollisionComponent();

	CollisionObjectHandle mCollisionObjectHandle;
};

struct GravityComponent : public ComponentBase
{
	RTTI_CLASS(GravityComponent, ComponentBase)

	GravityComponent() = default;
	GravityComponent(float inGravity) : mGravity(inGravity) {}

	// Increase velocity by gravity per second
	float mGravity = 100.f;
};

struct VelocityComponent : public ComponentBase
{
	RTTI_CLASS(VelocityComponent, ComponentBase)

	VelocityComponent() = default;
	VelocityComponent(const fm::vec2& inVelocity) : mVelocity(inVelocity) {}

	fm::vec2 mVelocity = {};
};

struct TransformComponent : public ComponentBase
{
	RTTI_CLASS(TransformComponent, ComponentBase)

	TransformComponent() = default;
	TransformComponent(const fm::Transform2D& inTransform) : mTransform(inTransform) {}

	fm::Transform2D mTransform = {};
};

struct HealthComponent : public ComponentBase
{
	RTTI_CLASS(HealthComponent, ComponentBase)

	HealthComponent() = default;
	HealthComponent(float inHealth) : mHealth(inHealth) {}

	float mHealth = 100.f;
};

struct CameraComponent : public ComponentBase
{
	RTTI_CLASS(CameraComponent, ComponentBase)

	CameraComponent();
	CameraComponent(SharedPtr<Camera> inCamera) : mCamera(inCamera) {}

	bool mIsActive = false; ///< If the camera is active, it will render based on priority
	int32 mPriority = 0; ///< Higher priority cameras will render on top of lower priority cameras
	SharedPtr<Camera> mCamera = nullptr;
};

// Tag components
struct DestroyedTag : public ComponentBase
{
	RTTI_CLASS(DestroyedTag, ComponentBase)

	DestroyedTag() = default;
	DestroyedTag(const UID& inUID) : mUID(inUID) {}

	UID mUID;
};
