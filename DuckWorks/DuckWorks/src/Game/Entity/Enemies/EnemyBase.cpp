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
	JSON_TRY_LOAD(inJson, mMaxVelocity);
	JSON_TRY_LOAD(inJson, mAcceleration);

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

	SetHalfSize(fm::vec2(64.f, 64.f));

	CollisionComponent::ConstructParameters collision_component_params;
	collision_component_params.mType = CollisionObject::EType::Dynamic;
	AddComponent<CollisionComponent>(collision_component_params);
}

void EnemyBase::Init()
{
	Base::Init();
}

void EnemyBase::Update(float inDeltaTime)
{
	Base::Update(inDeltaTime);

	if (!mPlayer.IsAlive())
		return;

	Ref<Player> player = mPlayer.Get();
	fm::vec2 direction = player->GetPosition() - GetPosition();
	direction = direction.normalize();

	mVelocity = mVelocity + inDeltaTime * mAcceleration;
	mVelocity = fm::min(mVelocity, mMaxVelocity);

	SetPosition(GetPosition() + direction * mVelocity * inDeltaTime);
}
