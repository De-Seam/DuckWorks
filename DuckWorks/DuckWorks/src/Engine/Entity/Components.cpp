#include "Precomp.h"
#include "Engine/Entity/Components.h"

#include "Engine/Renderer/Camera.h"
#include "Engine/Resources/ResourceManager.h"
#include "Engine/World/World.h"

#include "Game/App/App.h"

#include "External/box2d/box2d.h"

// NameComponent
Json NameComponent::Serialize() const
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mName);
	return json;
}

void NameComponent::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	JSON_TRY_LOAD(inJson, mName);
}

// TextureRenderComponent
Json TextureRenderComponent::Serialize() const
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

	mSrcRect = {inJson["mSrcRect"][0], inJson["mSrcRect"][1], inJson["mSrcRect"][2], inJson["mSrcRect"][3]};
	JSON_TRY_LOAD(inJson, mUseSrcRect);
	JSON_TRY_LOAD(inJson, mFlip);
}

TextureRenderComponent::TextureRenderComponent()
{
	mTexture = gResourceManager.GetResource<TextureResource>("Assets/DefaultTexture.png");
}

// AnimationComponent
Json AnimationComponent::Serialize() const
{
	return Base::Serialize();
}

void AnimationComponent::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);
}

// CollisionComponent
Json CollisionComponent::Serialize() const
{
	Json json = Base::Serialize();

	if (!mCollisionObjectHandle.IsValid())
		return json;

	json.update(gApp.GetWorld()->GetCollisionWorld()->GetCollisionObject(mCollisionObjectHandle).Serialize());

	return json;
}

void CollisionComponent::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	CollisionObject::InitParams params;
	if (!mCollisionObjectHandle.IsValid())
		mCollisionObjectHandle = gApp.GetWorld()->GetCollisionWorld()->CreateCollisionObject(params);
	gApp.GetWorld()->GetCollisionWorld()->DeserializeCollisionObject(mCollisionObjectHandle, inJson);
}

CollisionComponent::CollisionComponent()
{
	CollisionObject::InitParams params;
	mCollisionObjectHandle = gApp.GetWorld()->GetCollisionWorld()->CreateCollisionObject(params);
}

// TransformComponent
Json TransformComponent::Serialize() const
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mTransform);

	return json;
}

void TransformComponent::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	JSON_LOAD(inJson, mTransform);
}

// HealthComponent
Json HealthComponent::Serialize() const
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
Json CameraComponent::Serialize() const
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

CameraComponent::CameraComponent()
{}
