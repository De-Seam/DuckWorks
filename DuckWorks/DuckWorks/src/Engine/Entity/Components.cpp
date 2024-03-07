#include "Precomp.h"
#include "Engine/Entity/Components.h"

#include "Engine/Resources/ResourceManager.h"

Json NameComponent::Serialize() const
{
	Json json;
	JSON_SAVE(json, mName);
	return json;
}

void NameComponent::Deserialize(const Json& inJson)
{
	JSON_LOAD(inJson, mName);
}

TextureRenderComponent::TextureRenderComponent()
{
	mTexture = gResourceManager.GetResource<TextureResource>("Assets/DefaultTexture.png");
}

Json TextureRenderComponent::Serialize() const
{
	Json json;
	json["mTexture"] = mTexture->GetFileName();
	json["mSrcRect"] = {mSrcRect.x, mSrcRect.y, mSrcRect.z, mSrcRect.w};
	JSON_SAVE(json, mUseSrcRect);
	JSON_SAVE(json, mFlip);
	return json;
}

void TextureRenderComponent::Deserialize(const Json& inJson)
{
	mTexture = gResourceManager.GetResource<TextureResource>(inJson["mTexture"]);
	if (mTexture == nullptr)
		mTexture = gResourceManager.GetResource<TextureResource>("Assets/DefaultTexture.png");

	mSrcRect = {inJson["mSrcRect"][0], inJson["mSrcRect"][1], inJson["mSrcRect"][2], inJson["mSrcRect"][3]};
	JSON_LOAD(inJson, mUseSrcRect);
	JSON_LOAD(inJson, mFlip);
}

Json AnimationComponent::Serialize() const
{
	return {};
}

void AnimationComponent::Deserialize(const Json&)
{}

Json PhysicsComponent::Serialize() const
{
	Json json;
	json["mHalfSize"] =
	{
		mHalfSize.x, mHalfSize.y
	};
	return json;
}

void PhysicsComponent::Deserialize(const Json& inJson)
{
	if (inJson.contains("mHalfSize"))
	{
		mHalfSize.x = inJson["mHalfSize"][0];
		mHalfSize.y = inJson["mHalfSize"][1];
	}
}

Json TransformComponent::Serialize() const
{
	Json json;
	json["mTransform"] =
	{
		mTransform.position.x, mTransform.position.y,
		mTransform.halfSize.x,
		mTransform.halfSize.y, mTransform.rotation
	};
	return json;
}

void TransformComponent::Deserialize(const Json& inJson)
{
	mTransform.position.x = inJson["mTransform"][0];
	mTransform.position.y = inJson["mTransform"][1];
	mTransform.halfSize.x = inJson["mTransform"][2];
	mTransform.halfSize.y = inJson["mTransform"][3];
	mTransform.rotation = inJson["mTransform"][4];
}

Json HealthComponent::Serialize() const
{
	Json json;
	JSON_SAVE(json, mHealth);
	return json;
}

void HealthComponent::Deserialize(const Json& inJson)
{
	JSON_LOAD(inJson, mHealth);
}
