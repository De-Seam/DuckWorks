#pragma once
#include "Core/CoreBase.h"

// Engine includes
#include "Engine/Collision/CollisionStructs.h"

struct CollisionInfo
{
	bool mCollides;
	fm::vec2 mDirection;
	float mDepth;

	CollisionInfo(bool c, fm::vec2 dir, float d) : mCollides(c), mDirection(dir), mDepth(d) {}
};

bool gFullyInsideOf(const AABB& inA, const AABB& inB);
bool gCollides(const AABB& inA, const AABB& inB);
bool gCollides(const fm::vec2& inPoint, const AABB& inAABB);
CollisionInfo gCollides(const fm::Transform2D& inTransformA, const fm::Transform2D& inTransformB);

AABB gComputeAABB(const fm::Transform2D& inTransform);
AABB gComputeEncompassingAABB(const AABB& inA, const AABB& inB);
fm::Transform2D gComputeSweptShape(const fm::Transform2D& inOriginalTransform, const fm::vec2& inNewPosition, float inNewRotation);
