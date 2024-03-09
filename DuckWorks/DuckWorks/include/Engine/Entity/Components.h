#pragma once
// Core includes
#include "Core/Utilities/Utilities.h"
#include "Core/Math/FMath.h"

// Engine includes
#include "Core/Utilities/UID.h"

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

inline Json ComponentBase::Serialize() const { return Base::Serialize(); }
inline void ComponentBase::Deserialize(const Json& inJson) { Base::Deserialize(inJson); }

// EntityComponent is added to each entity to point towards the weak ptr entity
struct EntityComponent : public ComponentBase
{
	RTTI_CLASS(EntityComponent, ComponentBase)

	EntityComponent() = default;
	EntityComponent(WeakPtr<Entity> mEntity) : mEntity(mEntity) {}

	WeakPtr<Entity> mEntity;
};

inline Json EntityComponent::Serialize() const { return Base::Serialize(); }
inline void EntityComponent::Deserialize(const Json& inJson) { Base::Deserialize(inJson); }

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

struct PhysicsComponent : public ComponentBase
{
	RTTI_CLASS(PhysicsComponent, ComponentBase)

	PhysicsComponent() = default;
	PhysicsComponent(b2Body* inBody) : mBody(inBody) {}

	fm::vec2 mHalfSize = {32.f, 32.f}; ///< Can be equal to the transform, or a custom half size.
	fm::vec2 mOffset = {0.f, 0.f}; ///< Offset from the transform position
	b2Body* mBody = nullptr;
};

struct TransformComponent : public ComponentBase
{
	RTTI_CLASS(TransformComponent, ComponentBase)

	TransformComponent() = default;
	TransformComponent(fm::Transform2D inTransform) : mTransform(inTransform) {}

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

// This tag is used to mark entities that have been teleported and need to update their physics position.
// They will automatically get cleared after usage
struct PhysicsPositionOrRotationUpdatedTag : public ComponentBase
{
	RTTI_CLASS(PhysicsPositionOrRotationUpdatedTag, ComponentBase)

	PhysicsPositionOrRotationUpdatedTag() = default;
};

inline Json PhysicsPositionOrRotationUpdatedTag::Serialize() const { return Base::Serialize(); }
inline void PhysicsPositionOrRotationUpdatedTag::Deserialize(const Json& inJson) { Base::Deserialize(inJson); }

struct PhysicsSizeUpdatedTag : public ComponentBase
{
	RTTI_CLASS(PhysicsSizeUpdatedTag, ComponentBase)

	PhysicsSizeUpdatedTag() = default;
};

inline Json PhysicsSizeUpdatedTag::Serialize() const { return Base::Serialize(); }
inline void PhysicsSizeUpdatedTag::Deserialize(const Json& inJson) { Base::Deserialize(inJson); }

struct DestroyedTag : public ComponentBase
{
	RTTI_CLASS(DestroyedTag, ComponentBase)

	DestroyedTag() = default;
	DestroyedTag(const UID& inUID) : mUID(inUID) {}

	UID mUID;
};

inline Json DestroyedTag::Serialize() const { return Base::Serialize(); }
inline void DestroyedTag::Deserialize(const Json& inJson) { Base::Deserialize(inJson); }
