#include "Precomp.h"
#include "Game/Entity/Enemies/EnemyBase.h"

RTTI_CLASS_DECLARATION(EnemyBase)

RTTI_EMPTY_SERIALIZE_DEFINITION(EnemyBase)

EnemyBase::EnemyBase(World* inWorld)
	: Base(inWorld)
{

}

void EnemyBase::Update(float inDeltaTime)
{

}
