#include "Precomp.h"
#include "Game/Entity/Enemies/EnemyBase.h"

RTTI_CLASS_DEFINITION(EnemyBase, StandardAllocator)

RTTI_EMPTY_SERIALIZE_DEFINITION(EnemyBase)

void EnemyBase::Init()
{
	Base::Init();
}

void EnemyBase::Update(float inDeltaTime)
{
	Base::Update(inDeltaTime);
}
