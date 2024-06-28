#pragma once
#include "Core/CoreBase.h"

// Game includes
#include "EnemyBase.h"

class Zombie : public EnemyBase
{
	// Class Allocator for faster looping over lots of zombies
	RTTI_CLASS(Zombie, EnemyBase, ClassAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	Zombie(const ConstructParameters& inConstructParameters);
};