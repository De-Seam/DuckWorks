#include "Precomp.h"
#include "Game/Entity/MovingPlatform.h"

// Engine includes
#include "Engine/World/World.h"
#include "Engine/Collision/CollisionWorld.h"

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
	collision_component.mCollisionObjectHandle = GetWorld()->GetCollisionWorld()->CreateCollisionObject(CollisionObject::InitParams(transform, CollisionObject::Type::Dynamic, true));
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

	fm::vec2 position = GetPosition();

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

	position = GetWorld()->GetCollisionWorld()->MoveTo(GetComponent<CollisionComponent>().mCollisionObjectHandle, position);
	SetPosition(position);
}
