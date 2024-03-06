#pragma once
// Core includes
#include "Core/Utilities/Utilities.h"
#include "Core/Math/FMath.h"

// Engine includes
#include "Engine/Resources/ResourceTypes/TextureResource.h"
#include "Engine/Renderer/AnimationBase.h"

class b2Body;

struct ComponentBase : public RTTIBaseClass
{
	RTTI_CLASS(ComponentBase, RTTIBaseClass)

	virtual Json Serialize() const = 0;
	virtual void Deserialize(const Json& inJson) = 0;

	// This is a base class for all components
	// It is used to allow for polymorphism in the component manager
	// It is not meant to be used directly
};

struct NameComponent : public ComponentBase
{
	RTTI_CLASS(NameComponent, ComponentBase)

	NameComponent() = default;
	NameComponent(String inName) : mName(inName) {}

	virtual Json Serialize() const override;
	virtual void Deserialize(const Json& inJson) override;

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

	virtual Json Serialize() const override;
	virtual void Deserialize(const Json& inJson) override;

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

	virtual Json Serialize() const override;
	virtual void Deserialize(const Json& inJson) override;

	SharedPtr<AnimationBase> mAnimation = nullptr;

	// Private variables
	AnimationBase::Frame mCurrentFrame = {};
	float mTimeSinceUpdate = 0.f;
};

struct PhysicsComponent : public ComponentBase
{
	RTTI_CLASS(PhysicsComponent, ComponentBase)

	PhysicsComponent() = default;
	PhysicsComponent(b2Body* inBody) : mBody(inBody) {}

	virtual Json Serialize() const override;
	virtual void Deserialize(const Json& inJson) override;

	b2Body* mBody = nullptr;
};

struct TransformComponent : public ComponentBase
{
	RTTI_CLASS(TransformComponent, ComponentBase)

	TransformComponent() = default;
	TransformComponent(fm::Transform2D inTransform) : mTransform(inTransform) {}

	virtual Json Serialize() const override;
	virtual void Deserialize(const Json& inJson) override;

	fm::Transform2D mTransform = {};
};

struct HealthComponent : public ComponentBase
{
	RTTI_CLASS(HealthComponent, ComponentBase)

	HealthComponent() = default;
	HealthComponent(float inHealth) : mHealth(inHealth) {}

	virtual Json Serialize() const override;
	virtual void Deserialize(const Json& inJson) override;

	float mHealth = 100.f;
};

// Tag components

// This tag is used to mark entities that have been teleported and need to update their physics position.
// They will automatically get cleared after usage
struct PhysicsPositionUpdatedTag : public ComponentBase
{
	RTTI_CLASS(PhysicsPositionUpdatedTag, ComponentBase)

	PhysicsPositionUpdatedTag() = default;

	virtual Json Serialize() const override;
	virtual void Deserialize(const Json& inJson) override;
};
