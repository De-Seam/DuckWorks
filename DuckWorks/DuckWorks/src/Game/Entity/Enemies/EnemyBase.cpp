#include "Precomp.h"
#include "Game/Entity/Enemies/EnemyBase.h"

RTTI_CLASS_DEFINITION(EnemyBase)

RTTI_EMPTY_SERIALIZE_DEFINITION(EnemyBase)

void EnemyBase::Init(const Entity::InitParams& inInitParams)
{
	Base::Init(inInitParams);
}

void EnemyBase::Update(float inDeltaTime)
{
	Base::Update(inDeltaTime);
}
