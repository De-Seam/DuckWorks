#pragma once
// Core includes
#include "Core/Math/Math.h"
#include "Core/RTTI/Messages.h"
#include "Core/Utilities/UID.h"
#include "Core/Utilities/Utilities.h"

// Engine includes
#include "Engine/Entity/EntityMessages.h"
#include "Engine/Entity/Components/EntityComponent.h"
#include "Engine/Renderer/AnimationBase.h"
#include "Engine/Resources/ResourceTypes/TextureResource.h"

class Camera;
class b2Body;
class Entity;

#pragma warning( push )
#pragma warning( disable : 4324) // Structure was padded due to alignment specifier

// A component in the World, with a Transform
class WorldComponent : public EntityComponent
{
	RTTI_VIRTUAL_CLASS(WorldComponent, EntityComponent)
public:
	virtual Json Serialize() const override;
	virtual void Deserialize(const Json& inJson) override;

	WorldComponent();
	virtual ~WorldComponent() override;

	void SetLocalOffset(const Transform2D& inLocalOffset);

	const Transform2D& GetWorldTransform() const { return mWorldTransform; }
	const Transform2D& GetLocalOffset() const { return mLocalOffset; }

	void OnPostEntityPositionUpdated(const MsgPostEntityPositionUpdated& inMsg);
	void OnPostEntityRotationUpdated(const MsgPostEntityRotationUpdated& inMsg);

protected:
	virtual void CalculateWorldTransform();

private:
	Transform2D mLocalOffset;
	Transform2D mWorldTransform;
};

// Texture render component uses TransformComponent for its transform
class TextureRenderComponent : public WorldComponent
{
	RTTI_CLASS(TextureRenderComponent, WorldComponent)
public:
	TextureRenderComponent();

	SharedPtr<TextureResource> mTexture;
	IVec4 mSrcRect = {};
	bool mUseSrcRect = false;

	// Set by the animation component if it has one
	SDL_RendererFlip mFlip = SDL_FLIP_NONE;
};

// Animation component pairs with TextureRenderComponent
struct AnimationComponent : public EntityComponent
{
	RTTI_CLASS(AnimationComponent, EntityComponent)
public:
	AnimationComponent();

	virtual void Update(float inDeltaTime) override;

private:
	// RTTI variables
	SharedPtr<AnimationBase> mAnimation = nullptr;

	AnimationBase::Frame mCurrentFrame = {};
	float mTimeSinceUpdate = 0.f;
};

struct HealthComponent : public EntityComponent
{
	RTTI_CLASS(HealthComponent, EntityComponent)
public:
	HealthComponent();

	float mHealth = 100.f;
};

struct CameraComponent : public EntityComponent
{
	RTTI_CLASS(CameraComponent, EntityComponent)
public:
	CameraComponent();

	SharedPtr<Camera> mCamera = nullptr;
	int32 mPriority = 0; ///< Higher priority cameras will render on top of lower priority cameras
	bool mIsActive = false; ///< If the camera is active, it will render based on priority
};
