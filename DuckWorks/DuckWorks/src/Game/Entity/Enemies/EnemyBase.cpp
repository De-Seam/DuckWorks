#include "Precomp.h"
#include "Game/Entity/Enemies/EnemyBase.h"

// Engine includes
#include "Engine/Entity/Components/CollisionComponent.h"
#include "Engine/World/World.h"

// Game includes
#include "Game/Entity/Player/Player.h"

RTTI_CLASS_DEFINITION(EnemyBase, StandardAllocator)

Json EnemyBase::Serialize()
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mAcceleration);
	JSON_SAVE(json, mMaxVelocity);

	return json;
}

void EnemyBase::Deserialize(const Json& inJson)
{
	JSON_LOAD(inJson, mMaxVelocity);
	JSON_LOAD(inJson, mAcceleration);

	Base::Deserialize(inJson);
}

EnemyBase::EnemyBase(const ConstructParameters& inConstructParameters)
	: Base(inConstructParameters)
{
	for (Ref<Entity>& entity : GetWorld()->GetEntities())
	{
		if (entity->IsA<Player>())
		{
			mPlayer = entity.Cast<Player>();
			break;
		}
	}

	CollisionComponent::ConstructParameters collision_component_params;
	collision_component_params.mType = CollisionObject::EType::Dynamic;
	collision_component_params.mLocalOffset.mHalfSize = Vec2{32.f, 32.f};
	AddComponent<CollisionComponent>(collision_component_params);
}

void EnemyBase::Init()
{
	Base::Init();
}

void EnemyBase::Update(float inDeltaTime)
{
	PROFILE_SCOPE(EnemyBase::Update)

	Base::Update(inDeltaTime);

	if (!mPlayer.IsAlive())
		return;

	Ref<Player> player = mPlayer.Get();
	Vec2 direction = player->GetPosition() - GetPosition();
	direction = direction.Normalize();

	mVelocity = mVelocity + inDeltaTime * mAcceleration;
	mVelocity = gMin(mVelocity, mMaxVelocity);

	SetPosition(GetPosition() + direction * mVelocity * inDeltaTime);
}
