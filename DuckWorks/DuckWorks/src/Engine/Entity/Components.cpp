#include "Precomp.h"
#include "Engine/Entity/Components.h"

#include "Engine/Renderer/Camera.h"
#include "Engine/Resources/ResourceManager.h"
#include "Engine/World/World.h"

#include "Game/App/App.h"

// ComponentBase
RTTI_CLASS_DEFINITION(ComponentBase)

RTTI_EMPTY_SERIALIZE_DEFINITION(ComponentBase)

// NameComponent
RTTI_CLASS_DEFINITION(NameComponent)
COMPONENT_DEFINITION(NameComponent)

Json NameComponent::Serialize()
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
RTTI_CLASS_DEFINITION(TextureRenderComponent)
COMPONENT_DEFINITION(TextureRenderComponent)

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

	mSrcRect = {inJson["mSrcRect"][0], inJson["mSrcRect"][1], inJson["mSrcRect"][2], inJson["mSrcRect"][3]};
	JSON_TRY_LOAD(inJson, mUseSrcRect);
	JSON_TRY_LOAD(inJson, mFlip);
}

TextureRenderComponent::TextureRenderComponent()
{
	mTexture = gResourceManager.GetResource<TextureResource>("Assets/DefaultTexture.png");
}

// AnimationComponent
RTTI_CLASS_DEFINITION(AnimationComponent)
COMPONENT_DEFINITION(AnimationComponent)

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
COMPONENT_DEFINITION(CollisionComponent)

Json CollisionComponent::Serialize()
{
	Json json = Base::Serialize();

	if (!mCollisionObjectHandle.IsValid())
		return json;

	CollisionObjectWrapper collision_object = gApp.GetWorld()->GetCollisionWorld()->GetCollisionObject(mCollisionObjectHandle);
	json.update(collision_object->Serialize());

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
{}

CollisionComponent::~CollisionComponent()
{
}

// TransformComponent
RTTI_CLASS_DEFINITION(TransformComponent)
COMPONENT_DEFINITION(TransformComponent)

Json TransformComponent::Serialize()
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

// Entity Component
RTTI_CLASS_DEFINITION(EntityRefComponent)

COMPONENT_DEFINITION(EntityRefComponent)

RTTI_EMPTY_SERIALIZE_DEFINITION(EntityRefComponent)

// HealthComponent
RTTI_CLASS_DEFINITION(HealthComponent)

COMPONENT_DEFINITION(HealthComponent)

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

COMPONENT_DEFINITION(CameraComponent)

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

CameraComponent::CameraComponent()
{}

// DestroyedTag
RTTI_CLASS_DEFINITION(DestroyedTag)

COMPONENT_DEFINITION(DestroyedTag)

RTTI_EMPTY_SERIALIZE_DEFINITION(DestroyedTag)
