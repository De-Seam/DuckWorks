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

	JSON_LOAD(inJson, mName);
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

// PhysicsComponent
Json PhysicsComponent::Serialize() const
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mBody);
	//to_json(json["mBody"], mBody);
	JSON_SAVE(json, mHalfSize);
	JSON_SAVE(json, mOffset);

	return json;
}

void PhysicsComponent::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	if (inJson.contains("mBody"))
		FromJson(inJson["mBody"], mBody);
	JSON_TRY_LOAD(inJson, mHalfSize);
	JSON_TRY_LOAD(inJson, mOffset);
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

PhysicsComponent::PhysicsComponent()
{
	b2BodyDef body_def;
	body_def.type = b2_dynamicBody;
	body_def.position = {0.f, 0.f};
	body_def.angle = {0.f};
	body_def.linearVelocity = b2Vec2(0.0f, 0.0f);
	body_def.linearDamping = 0.3f;

	b2PolygonShape dynamic_box;
	dynamic_box.SetAsBox(32.f, 32.f);

	b2FixtureDef fixture_def;
	fixture_def.shape = &dynamic_box;
	fixture_def.density = 1.0f;
	fixture_def.friction = 1.f;
	fixture_def.restitutionThreshold = 100000.f;
	fixture_def.isSensor = true;

	mBody = gApp.GetWorld()->CreatePhysicsBody(body_def, fixture_def);
}

PhysicsComponent::PhysicsComponent(b2Body* inBody)
	: mBody(inBody)
{}

PhysicsComponent::PhysicsComponent(b2Body* inBody, fm::vec2 inHalfSize, fm::vec2 inOffset)
	: mBody(inBody)
	, mHalfSize(inHalfSize)
	, mOffset(inOffset)
{}
