#pragma once
#include "Core/CoreBase.h"

// Engine includes
#include "Engine/Collision/CollisionStructs.h"

struct BVHNode
{
	AABB mAABB;
	uint32 mLeftFirst;
	uint32 mCount;
};