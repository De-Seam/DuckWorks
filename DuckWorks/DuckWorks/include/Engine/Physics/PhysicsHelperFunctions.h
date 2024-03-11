#pragma once
#include "Core/CoreBase.h"

// Engine includes
#include "Engine/Physics/PhysicStructs.h"

bool gCollides(const AABB& inA, const AABB& inB);
bool gCollides(const fm::Transform2D& inTransformA, const fm::Transform2D& inTransformB);

AABB gComputeAABB(const fm::Transform2D inTransform);