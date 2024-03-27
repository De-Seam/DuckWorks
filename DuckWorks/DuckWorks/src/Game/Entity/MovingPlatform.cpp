#include "Precomp.h"
#include "Game/Entity/MovingPlatform.h"

// Engine includes
#include "Engine/World/World.h"
#include "Engine/Collision/CollisionWorld.h"
#include "Engine/Resources/ResourceManager.h"

// Game includes
#include "Game/Entity/Player/Player.h"

Json MovingPlatform::Serialize() const
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

MovingPlatform::MovingPlatform(World* inWorld)
	: Base(inWorld)
{
	fm::Transform2D& transform = GetComponent<TransformComponent>().mTransform;

	CollisionComponent& collision_component = AddComponent<CollisionComponent>();
	CollisionObject::InitParams init_params;
	init_params.mTransform = transform;
	init_params.mType = CollisionObject::EType::Dynamic;
	collision_component.mCollisionObjectHandle = GetWorld()->GetCollisionWorld()->CreateCollisionObject(
		CollisionObject::InitParams(transform, CollisionObject::EType::Dynamic, true));

	AddComponent<TextureRenderComponent>().mTexture = gResourceManager.GetResource<TextureResource>("Assets/top.jpg");
}

void MovingPlatform::BeginPlay()
{
	Base::BeginPlay();

	if (mStartPosition.x == 0.f && mStartPosition.y == 0.f)
		mStartPosition = GetPosition();
}

void MovingPlatform::Update(float inDeltaTime)
{
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

	fm::Transform2D transform = GetComponent<TransformComponent>().mTransform;
	transform.position = position;
	const Array<CollisionData>& collision_data = GetWorld()->GetCollisionWorld()->CheckCollisions(transform);
	for (const CollisionData& data : collision_data)
	{
		if (data.mHandle == GetComponent<CollisionComponent>().mCollisionObjectHandle)
			continue;

		if (data.mHandle.IsValid())
		{
			EntityPtr entity = GetWorld()->GetCollisionWorld()->GetCollisionObject(data.mHandle)->GetEntity().lock();
			if (entity != nullptr)
			{
				if (strcmp(entity->GetClassName(), "Player") == 0)
				{
					SharedPtr<Player> player = SPCast<Player>(entity);
					fm::vec2 player_position = player->GetPosition();
					player_position += (position - old_position);
					player->SetPosition(GetWorld()->GetCollisionWorld()->MoveTo(data.mHandle, player_position).position);
				}
			}
		}
	}

	transform = GetWorld()->GetCollisionWorld()->MoveTo(GetComponent<CollisionComponent>().mCollisionObjectHandle, position);
	SetPosition(transform.position);
}
