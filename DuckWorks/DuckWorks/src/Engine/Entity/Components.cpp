#include "Precomp.h"
#include "Engine/Entity/Components.h"

// Engine includes
#include "Engine/Renderer/Camera.h"
#include "Engine/Resources/ResourceManager.h"
#include "Engine/World/World.h"

// WorldComponent
RTTI_VIRTUAL_CLASS_DEFINITION(WorldComponent)

Json WorldComponent::Serialize()
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mTransform);

	return json;
}

void WorldComponent::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	JSON_TRY_LOAD(inJson, mTransform);
}

// TextureRenderComponent
RTTI_CLASS_DEFINITION(TextureRenderComponent, ClassAllocator)

WorldComponent::WorldComponent(const ConstructParameters& inConstructParameters)
	: Base(inConstructParameters)
{
	if (inConstructParameters.mTransform.mPosition != Vec2(0.f, 0.f))
		mTransform.mPosition = inConstructParameters.mTransform.mPosition;
	if (inConstructParameters.mTransform.mRotation != 0.f)
		mTransform.mRotation = inConstructParameters.mTransform.mRotation;

	mTransform.mHalfSize = inConstructParameters.mTransform.mHalfSize;

	GetEntity()->RegisterMessageListener(this, &WorldComponent::OnPostEntityPositionUpdated);
	GetEntity()->RegisterMessageListener(this, &WorldComponent::OnPostEntityRotationUpdated);
}

WorldComponent::~WorldComponent()
{
	GetEntity()->UnregisterMessageListener(this, &WorldComponent::OnPostEntityPositionUpdated);
	GetEntity()->UnregisterMessageListener(this, &WorldComponent::OnPostEntityRotationUpdated);
}

void WorldComponent::OnPostEntityPositionUpdated(const MsgPostEntityPositionUpdated& inMsg)
{
	mTransform.mPosition = inMsg.mNewPosition;
}

void WorldComponent::OnPostEntityRotationUpdated(const MsgPostEntityRotationUpdated& inMsg)
{
	mTransform.mRotation = inMsg.mNewRotation;
}

Json TextureRenderComponent::Serialize()
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mTexture);
	JSON_SAVE(json, mTransform);
	json["mSrcRect"] = {mSrcRect.mX, mSrcRect.mY, mSrcRect.mZ, mSrcRect.mW};
	JSON_SAVE(json, mUseSrcRect);
	JSON_SAVE(json, mFlip);

	return json;
}

void TextureRenderComponent::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	JSON_TRY_LOAD(inJson, mTexture);
	JSON_TRY_LOAD(inJson, mTransform);

	if (inJson.contains("mSrcRect"))
		mSrcRect = {inJson["mSrcRect"][0], inJson["mSrcRect"][1], inJson["mSrcRect"][2], inJson["mSrcRect"][3]};
	JSON_TRY_LOAD(inJson, mUseSrcRect);
	JSON_TRY_LOAD(inJson, mFlip);
}

TextureRenderComponent::TextureRenderComponent(const ConstructParameters& inConstructParameters)
	: Base(inConstructParameters),
	mTexture(inConstructParameters.mTexture),
	mSrcRect(inConstructParameters.mSrcRect),
	mUseSrcRect(inConstructParameters.mUseSrcRect),
	mFlip(inConstructParameters.mFlip)
{
	if (mTexture == nullptr)
		mTexture = gResourceManager.GetResource<TextureResource>("Assets/DefaultTexture.png");
}

// AnimationComponent
RTTI_CLASS_DEFINITION(AnimationComponent, ClassAllocator)

Json AnimationComponent::Serialize()
{
	return Base::Serialize();
}

void AnimationComponent::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);
}

void AnimationComponent::Update(float inDeltaTime)
{
	gDebugIf(mAnimation == nullptr, return)

	TextureRenderComponent* render_component = GetEntity()->GetFirstComponentOfType<TextureRenderComponent>();
	gDebugIf(render_component == nullptr, return)

	mAnimation->Update(inDeltaTime);

	render_component->mUseSrcRect = true;
	mTimeSinceUpdate += inDeltaTime;

	AnimationBase::Frame& current_frame = mCurrentFrame;
	render_component->mFlip = mAnimation->GetFlip();
	gAssert(current_frame.mDuration > 0.f, "Duration should be higher then 0!");

	while (mTimeSinceUpdate >= current_frame.mDuration)
	{
		mTimeSinceUpdate -= current_frame.mDuration;
		current_frame = mAnimation->IncrementFrame();
		render_component->mSrcRect = {current_frame.mPosition, current_frame.mSize};
	}
}

// HealthComponent
RTTI_CLASS_DEFINITION(HealthComponent, ClassAllocator)

Json HealthComponent::Serialize()
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mHealth);

	return json;
}

void HealthComponent::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	JSON_LOAD(inJson, mHealth);
}

// CameraComponent
RTTI_CLASS_DEFINITION(CameraComponent, ClassAllocator)

Json CameraComponent::Serialize()
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mIsActive);
	JSON_SAVE(json, mPriority);
	if (mCamera != nullptr)
		json["mCamera"] = mCamera->Serialize();

	return json;
}

void CameraComponent::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	JSON_TRY_LOAD(inJson, mIsActive);
	JSON_TRY_LOAD(inJson, mPriority);

	if (inJson.contains("mCamera"))
	{
		if (mCamera == nullptr)
			mCamera = std::make_shared<Camera>();
		mCamera->Deserialize(inJson["mCamera"]);
	}
}

CameraComponent::CameraComponent(const ConstructParameters& inConstructParameters)
	: Base(inConstructParameters), mCamera(inConstructParameters.mCamera), mPriority(inConstructParameters.mPriority), mIsActive(inConstructParameters.mIsActive)
{
	if (mCamera == nullptr)
		mCamera = std::make_shared<Camera>();
}
