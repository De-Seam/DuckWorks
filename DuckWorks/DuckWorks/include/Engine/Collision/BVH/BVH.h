#pragma once
#include "Core/CoreBase.h"

// Engine includes
#include "BVHNode.h"
#include "Engine/Collision/CollisionStructs.h"

class CollisionWorld;

// BVH is a private class from the CollisionWorld, so public functions should be called from the CollisionWorld
class BVH
{
public:
	void AddObject(CollisionObjectHandle inObject);
	void RemoveObject(CollisionObjectHandle inObject);

	// Generate the BVH. Expensive operation.
	void Generate();

private:
	Mutex mBVHMutex; ///< For generating or resizing the BVH itself

	CollisionWorld* mCollisionWorld;

	Array<CollisionObjectHandle> mObjects;
	Array<BVHNode> mNodes;
	uint32* mIndices; ///< Indices of the objects in the mObjects array, uses int32 for space efficiency

private:
	// Creates AABB from objects. Uses mIndices.
	AABB mCreateAABBFromObjects(uint32 inFirst, uint32 inCount);

	void Subdivide(BVHNode* inNode, int32 inFirst, int32 inCount, int32 inDepth);
	void Partition(BVHNode* inNode, int32 inFirst, int32 inCount, int32 inDepth);
};