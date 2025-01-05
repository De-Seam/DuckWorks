#pragma once
#include <Game/Entity/Entity.h>

class Pawn : public Entity
{
	RTTI_CLASS(Pawn,Entity)
public:
	explicit Pawn(World& inWorld);

private:
	DC::String mName = "Pawn";

	float mHealth = 100.0f;

	float mSpeed = 1.0f;
};