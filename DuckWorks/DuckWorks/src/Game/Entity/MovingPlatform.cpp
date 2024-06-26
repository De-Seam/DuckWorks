#include "Precomp.h"
#include "Game/Entity/MovingPlatform.h"

// Engine includes
#include "Engine/Collision/CollisionWorld.h"
#include "Engine/Resources/ResourceManager.h"
#include "Engine/World/World.h"

// Game includes
#include "Game/Entity/Player/Player.h"

RTTI_CLASS_DEFINITION(MovingPlatform)

Json MovingPlatform::Serialize()
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mMoveExtents);
	JSON_SAVE(json, mMoveSpeed);
	JSON_SAVE(json, mStartPosition);

	return json;
}

void MovingPlatform::Deserialize(const Json& inJson)
{
	JSON_TRY_LOAD(inJson, mMoveExtents);
	JSON_TRY_LOAD(inJson, mMoveSpeed);
	JSON_TRY_LOAD(inJson, mStartPosition);

	Base::Deserialize(inJson);
}

void MovingPlatform::Init(const InitParams& inInitParams)
{
	Base::Init(inInitParams);

	{
		MutexReadProtectedPtr<CollisionComponent> collision_component = GetComponent<CollisionComponent>();
		CollisionObjectWrapper collision_object = GetWorld()->GetCollisionWorld()->GetCollisionObject(collision_component->mCollisionObjectHandle);
		collision_object->SetType(CollisionObject::EType::Dynamic);
	}

	AddComponent<TextureRenderComponent>();
	GetComponent<TextureRenderComponent>()->mTexture = gResourceManager.GetResource<TextureResource>("Assets/top.jpg");
}

void MovingPlatform::BeginPlay()
{
	Base::BeginPlay();

	if (mStartPosition.x == 0.f && mStartPosition.y == 0.f)
		mStartPosition = GetPosition();
}

void MovingPlatform::Update(float inDeltaTime)
{
	PROFILE_SCOPE(MovingPlatform::Update)

	Base::Update(inDeltaTime);

	fm::vec2 old_position = GetPosition();
	fm::vec2 position = old_position;

	position += mMoveSpeed * inDeltaTime;

	if (position.x > mStartPosition.x + mMoveExtents.x)
	{
		position.x = mStartPosition.x + mMoveExtents.x;
		mMoveSpeed.x *= -1.f;
	}
	else if (position.x < mStartPosition.x - mMoveExtents.x)
	{
		position.x = mStartPosition.x - mMoveExtents.x;
		mMoveSpeed.x *= -1.f;
	}

	if (position.y > mStartPosition.y + mMoveExtents.y)
	{
		position.y = mStartPosition.y + mMoveExtents.y;
		mMoveSpeed.y *= -1.f;
	}
	else if (position.y < mStartPosition.y - mMoveExtents.y)
	{
		position.y = mStartPosition.y - mMoveExtents.y;
		mMoveSpeed.y *= -1.f;
	}

	fm::Transform2D transform = GetTransform();
	transform.position = position;
	CollisionObjectHandle own_handle = GetComponent<CollisionComponent>()->mCollisionObjectHandle;
	const Array<CollisionData>& collision_data = GetWorld()->GetCollisionWorld()->CheckCollisions(transform);
	for (const CollisionData& data : collision_data)
	{
		if (data.mHandle == own_handle)
			continue;

		if (data.mHandle.IsValid())
		{
			WeakRef<Entity> entity_weakref = GetWorld()->GetCollisionWorld()->GetCollisionObject(data.mHandle)->GetEntity();

			if (!entity_weakref.IsAlive())
				continue;

			Ref<Entity> entity = entity_weakref.Get();

			CollisionActor* actor = entity.Cast<CollisionActor>();
			if (actor != nullptr)
			{
				fm::vec2 actor_position = actor->GetPosition();
				actor_position += (position - old_position);
				actor->MoveTo(actor_position);
			}
		}
	}

	MoveTo(transform.position);
}
