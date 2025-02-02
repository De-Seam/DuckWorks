#pragma once
#include "Core/CoreBase.h"

// Engine includes
#include "Engine/Collision/CollisionStructs.h"

bool gFullyInsideOf(const AABB& inA, const AABB& inB);
bool gCollides(const AABB& inA, const AABB& inB);
bool gCollides(const Vec2& inPoint, const AABB& inAABB);
bool gCollides(const Vec2& inPoint, const Transform2D& inTransform);
CollisionInfo gCollides(const Transform2D& inTransformA, const Transform2D& inTransformB);

AABB gComputeAABB(const Transform2D& inTransform);
AABB gComputeEncompassingAABB(const AABB& inA, const AABB& inB);
Transform2D gComputeSweptShape(const Transform2D& inOriginalTransform, const Vec2& inNewPosition, float inNewRotation);
Transform2D gComputeSweptShape(const Transform2D& inOriginalTransform, const Transform2D& inNewTransform);
