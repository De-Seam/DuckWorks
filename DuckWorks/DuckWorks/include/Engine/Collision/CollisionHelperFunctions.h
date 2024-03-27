#pragma once
#include "Core/CoreBase.h"

// Engine includes
#include "Engine/Collision/CollisionStructs.h"

bool gFullyInsideOf(const AABB& inA, const AABB& inB);
bool gCollides(const AABB& inA, const AABB& inB);
bool gCollides(const fm::vec2& inPoint, const AABB& inAABB);
CollisionInfo gCollides(const fm::Transform2D& inTransformA, const fm::Transform2D& inTransformB);

AABB gComputeAABB(const fm::Transform2D& inTransform);
AABB gComputeEncompassingAABB(const AABB& inA, const AABB& inB);
fm::Transform2D gComputeSweptShape(const fm::Transform2D& inOriginalTransform, const fm::vec2& inNewPosition, float inNewRotation);
fm::Transform2D gComputeSweptShape(const fm::Transform2D& inOriginalTransform, const fm::Transform2D& inNewTransform);
