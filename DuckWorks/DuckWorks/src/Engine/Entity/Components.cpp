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

	JSON_SAVE(json, mLocalOffset);

	return json;
}

void WorldComponent::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	JSON_LOAD(inJson, mLocalOffset);

	CalculateWorldTransform();
}

// TextureRenderComponent
RTTI_CLASS_DEFINITION(TextureRenderComponent, ClassAllocator)

WorldComponent::WorldComponent(const ConstructParameters& inConstructParameters)
	: Base(inConstructParameters), mLocalOffset(inConstructParameters.mLocalOffset)
{
	CalculateWorldTransform();

	GetEntity()->RegisterMessageListener(this, &WorldComponent::OnPostEntityPositionUpdated);
	GetEntity()->RegisterMessageListener(this, &WorldComponent::OnPostEntityRotationUpdated);
}

WorldComponent::~WorldComponent()
{
	GetEntity()->UnregisterMessageListener(this, &WorldComponent::OnPostEntityPositionUpdated);
	GetEntity()->UnregisterMessageListener(this, &WorldComponent::OnPostEntityRotationUpdated);
}

void WorldComponent::SetLocalOffset(const Transform2D& inLocalOffset)
{
	mLocalOffset = inLocalOffset;
	CalculateWorldTransform();
}

void WorldComponent::OnPostEntityPositionUpdated(const MsgPostEntityPositionUpdated& inMsg)
{
	mWorldTransform.mPosition = inMsg.mNewPosition + mLocalOffset.mPosition;
}

void WorldComponent::OnPostEntityRotationUpdated(const MsgPostEntityRotationUpdated& inMsg)
{
	mWorldTransform.mRotation = inMsg.mNewRotation + mLocalOffset.mRotation;
}

void WorldComponent::CalculateWorldTransform()
{
	mWorldTransform = mLocalOffset;
	mWorldTransform.mPosition += GetEntity()->GetPosition();
	mWorldTransform.mRotation += GetEntity()->GetRotation();
}

Json TextureRenderComponent::Serialize()
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mTexture);
	json["mSrcRect"] = {mSrcRect.mX, mSrcRect.mY, mSrcRect.mZ, mSrcRect.mW};
	JSON_SAVE(json, mUseSrcRect);
	JSON_SAVE(json, mFlip);

	return json;
}

void TextureRenderComponent::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	JSON_LOAD(inJson, mTexture);

	if (inJson.contains("mSrcRect"))
		mSrcRect = {inJson["mSrcRect"][0], inJson["mSrcRect"][1], inJson["mSrcRect"][2], inJson["mSrcRect"][3]};
	JSON_LOAD(inJson, mUseSrcRect);
	JSON_LOAD(inJson, mFlip);
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

	JSON_LOAD(inJson, mIsActive);
	JSON_LOAD(inJson, mPriority);

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
