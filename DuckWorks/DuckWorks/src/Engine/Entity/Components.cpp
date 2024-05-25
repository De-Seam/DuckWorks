#include "Precomp.h"
#include "Engine/Entity/Components.h"

#include "Engine/Renderer/Camera.h"
#include "Engine/Resources/ResourceManager.h"
#include "Engine/World/World.h"

#include "Game/App/App.h"

// TextureRenderComponent
RTTI_CLASS_DEFINITION(TextureRenderComponent)

Json TextureRenderComponent::Serialize()
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mTexture);
	json["mSrcRect"] = {mSrcRect.x, mSrcRect.y, mSrcRect.z, mSrcRect.w};
	JSON_SAVE(json, mUseSrcRect);
	JSON_SAVE(json, mFlip);

	return json;
}

void TextureRenderComponent::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	JSON_TRY_LOAD(inJson, mTexture);

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
RTTI_CLASS_DEFINITION(AnimationComponent)

Json AnimationComponent::Serialize()
{
	return Base::Serialize();
}

void AnimationComponent::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);
}

// CollisionComponent
RTTI_CLASS_DEFINITION(CollisionComponent)

Json CollisionComponent::Serialize()
{
	Json json = Base::Serialize();

	if (!mCollisionObjectHandle.IsValid())
		return json;

	CollisionObjectWrapper collision_object = gApp.GetWorld()->GetCollisionWorld()->GetCollisionObject(mCollisionObjectHandle);
	json["CollisionObject"] = collision_object->Serialize();

	return json;
}

void CollisionComponent::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	CollisionObject::ConstructParameters construct_params;
	if (!mCollisionObjectHandle.IsValid())
		mCollisionObjectHandle = gApp.GetWorld()->GetCollisionWorld()->CreateCollisionObject(construct_params);
	gApp.GetWorld()->GetCollisionWorld()->DeserializeCollisionObject(mCollisionObjectHandle, inJson["CollisionObject"]);
}

// HealthComponent
RTTI_CLASS_DEFINITION(HealthComponent)

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
RTTI_CLASS_DEFINITION(CameraComponent)

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
