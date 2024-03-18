#pragma once
#include "Core/CoreBase.h"

struct BVHNode
{
	AABB mAABB;
	int32 mLeftFirst;
	int32 mRightCount;
};