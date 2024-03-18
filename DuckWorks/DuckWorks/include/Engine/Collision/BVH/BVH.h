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
	
	// Broadphase collision detection. It returns a const ref to avoid having to keep allocating memory. Keep in mind not to call this in a nested loop as the previous returned array will be invalidated!
	const Array<CollisionObjectHandle>& GetBroadphaseCollisions(const AABB& inAABB);
private:
	Mutex mBVHMutex; ///< For generating or resizing the BVH itself

	CollisionWorld* mCollisionWorld;

	Array<CollisionObjectHandle> mObjects;
	Array<BVHNode> mNodes;
	uint32* mIndices; ///< Indices of the objects in the mObjects array, uses int32 for space efficiency

private:
	// Creates AABB from objects. Uses mIndices.
	AABB CreateAABBFromObjects(uint64 inFirst, uint64 inCount);

	void Subdivide(BVHNode* inNode, uint64 inFirst, uint64 inCount, uint64 inDepth);
	void Partition(BVHNode* inNode, uint64 inFirst, uint64 inCount, uint64 inDepth);
	float ComputeSplitCost(uint64 first, uint64 count, float split_location, uint64 split_axis);
	uint64 SplitIndices(uint64 inFirst, uint64 inCount, float inSplitLocation, uint64 inSplitAxis);

	void CollisionInternal(Array<CollisionObjectHandle>& ioReturnArray, const AABB& inAABB, uint64 inNodeIndex);
};