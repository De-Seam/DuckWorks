#include "Precomp.h"
#include "Engine/Entity/Components.h"

#include "Engine/Resources/ResourceManager.h"

Json NameComponent::Serialize() const
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mName);
	return json;
}

void NameComponent::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	JSON_LOAD(inJson, mName);
}

TextureRenderComponent::TextureRenderComponent()
{
	mTexture = gResourceManager.GetResource<TextureResource>("Assets/DefaultTexture.png");
}

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

	if (inJson.contains("mTexture"))
		mTexture = gResourceManager.GetResource<TextureResource>(inJson["mTexture"]["mFile"]);
	if (mTexture == nullptr)
		mTexture = gResourceManager.GetResource<TextureResource>("Assets/DefaultTexture.png");

	mSrcRect = {inJson["mSrcRect"][0], inJson["mSrcRect"][1], inJson["mSrcRect"][2], inJson["mSrcRect"][3]};
	JSON_TRY_LOAD(inJson, mUseSrcRect);
	JSON_TRY_LOAD(inJson, mFlip);
}

Json AnimationComponent::Serialize() const
{
	return Base::Serialize();
}

void AnimationComponent::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);
}

Json PhysicsComponent::Serialize() const
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mHalfSize);
	JSON_SAVE(json, mOffset);

	return json;
}

void PhysicsComponent::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	JSON_LOAD(inJson, mHalfSize);
	JSON_LOAD(inJson, mOffset);
}

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
