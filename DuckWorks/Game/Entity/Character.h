#pragma once
// Game includes
#include <Game/Entity/Entity.h>

class Character : public Entity
{
	RTTI_CLASS(Character, Entity)
public:
	Character(World* inWorld);
};