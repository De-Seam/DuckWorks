#pragma once
#include "Core/CoreBase.h"

// Engine includes
#include "Engine/Physics/PhysicsStructs.h"

bool gCollides(const AABB& inA, const AABB& inB);
bool gCollides(const fm::Transform2D& inTransformA, const fm::Transform2D& inTransformB);

AABB gComputeAABB(const fm::Transform2D& inTransform);
fm::Transform2D gComputeSweptShape(const fm::Transform2D& inOriginalTransform, const fm::vec2& inNewPosition, float inNewRotation);
