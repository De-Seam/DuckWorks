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
	BVH() = default;
	~BVH();

	struct InitParams
	{
		CollisionWorld* mCollisionWorld;
	};
	void Init(const InitParams& inParams);

	void Draw(); ///< Debug draw the BVH
	
	void AddObject(const CollisionObjectHandle& inObject);
	void AddObjects(const Array<CollisionObjectHandle>& inObjects);
	void RemoveObject(const CollisionObjectHandle& inObject);

	// Generate the BVH. Expensive operation.
	void Generate();

	bool IsGenerated() { return mIndices != nullptr; }

	// Refresh the object, adjusting its bounds
	void RefreshObject(const CollisionObjectHandle& inObject);
	
	// Broadphase collision detection. It returns a const ref to avoid having to keep allocating memory. Keep in mind not to call this in a nested loop as the previous returned array will be invalidated!
	const Array<CollisionObjectHandle>& GetBroadphaseCollisions(const AABB& inAABB);
private:
	Mutex mBVHMutex; ///< For generating or resizing the BVH itself

	CollisionWorld* mCollisionWorld;

	struct CollisionObjectData
	{
		AABB mAABB = { FLT_MAX, FLT_MIN }; ///< The AABB is usually slightly bigger then the collision object's handle so it can move without having to immediately resize the BVH
		CollisionObjectHandle mCollisionObjectHandle = {};
	};
	Array<CollisionObjectData> mObjects;
	Array<BVHNode> mNodes;
	uint32* mIndices = nullptr; ///< Indices of the objects in the mObjects array, uses int32 for space efficiency
	uint32 mIndexCount = 0; ///< Count of mIndices. Saved because mObjects can change before the mIndices is reconstructed

private:
	void AdjustAABBForDynamicObject(AABB& ioAABB);

	// Creates AABB from objects. Uses mIndices.
	AABB CreateAABBFromObjects(uint64 inFirst, uint64 inCount);

	void Subdivide(BVHNode* inNode, uint64 inFirst, uint64 inCount, uint64 inDepth);
	void Partition(BVHNode* inNode, uint64 inFirst, uint64 inCount, uint64 inDepth);
	float ComputeSplitCost(uint64 first, uint64 count, float split_location, uint64 split_axis);
	uint64 SplitIndices(uint64 inFirst, uint64 inCount, float inSplitLocation, uint64 inSplitAxis);

	void CollisionInternal(Array<CollisionObjectHandle>& ioReturnArray, const AABB& inAABB, uint64 inNodeIndex);
	const Array<uint64>& FindNodeHierarchyContainingObject(const CollisionObjectHandle& inObject);
	bool FindNodeHierarchyContainingObjectRecursive(Array<uint64>& ioIndices, const CollisionObjectHandle& inObject, const fm::vec2 inCenter, uint64 inNodeIndex);

	void ExpandNodeToFitAABB(BVHNode* ioNode, const AABB& inAABB);

	struct DrawData
	{
		AABB mAABB;
		uint64 mDepth;
		bool mLeaf = false;
	};
	void GetDrawDataRecursively(Array<DrawData>& ioDrawData, uint64 inNodeIndex, uint64 inDepth, uint64& ioMaxDepth);
};