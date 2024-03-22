#include "Precomp.h"
#include "Engine/Collision/BVH/BVH.h"

// Engine includes
#include "Engine/Collision/CollisionWorld.h"
#include "Engine/Collision/CollisionHelperFunctions.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Debug/DebugUIFunctions.h"

void BVH::Init(const InitParams& inParams)
{
	mCollisionWorld = inParams.mCollisionWorld;
}

BVH::~BVH()
{
	if (mIndices)
		_aligned_free(mIndices);
}

void BVH::AddObject(const CollisionObjectHandle& inObject)
{
	PROFILE_SCOPE(BVH::AddObject)

	bool resized = false;
	if (mObjects.size() <= inObject.mIndex)
	{
		mObjects.reserve(inObject.mIndex + 32); ///< Reserve some extra space to avoid reallocations
		mObjects.resize(inObject.mIndex + 1);
		resized = true;
	}
	else
		gAssert(mObjects[inObject.mIndex].mCollisionObjectHandle.mIndex == inObject.mIndex, "Previously added object somehow has the wrong index!");

	CollisionObjectData data;
	data.mCollisionObjectHandle = inObject;
	const CollisionObject& object = mCollisionWorld->GetCollisionObject(inObject);
	data.mAABB = object.GetAABB();
	if (object.GetType() == CollisionObject::EType::Dynamic)
	{
		AdjustAABBForDynamicObject(data.mAABB);
	}
	mObjects[inObject.mIndex] = data;

	if (IsGenerated())
	{
		if (!resized)
			RefreshObject(inObject);
		else
			Generate();
	}
}

void BVH::AddObjects(const Array<CollisionObjectHandle>& inObjects)
{
	for (const CollisionObjectHandle& object : inObjects)
		AddObject(object);
}

void BVH::RemoveObject(const CollisionObjectHandle& inObject)
{
	gAssert(mObjects[inObject.mIndex].mCollisionObjectHandle == inObject, "Trying to remove an object that was never added!");
	mObjects[inObject.mIndex] = {};
}

void BVH::Generate()
{
	PROFILE_SCOPE(BVH::Generate)

	ScopedMutexWriteLock lock(mBVHMutex);

	if (mIndices)
		_aligned_free(mIndices);

	mNodes.clear();

	mIndexCount = SCast<uint32>(mObjects.size());
	mIndices = SCast<uint32*>(_aligned_malloc((SCast<size_t>(mIndexCount) + 1) * sizeof(uint32), 64));
	for (size_t i = 0; i < mObjects.size(); i++)
		mIndices[i] = SCast<uint32>(i);

	mNodes.reserve(mObjects.size() * 2 + 1);
	mNodes.emplace_back(BVHNode()); //Root Node
	mNodes.emplace_back(BVHNode()); //Empty Node for cache alignment

	mNodes[0].mAABB = CreateAABBFromObjects(0, mObjects.size());
	Subdivide(&mNodes[0], 0, mObjects.size(), 0);

	gLog("BVH Created with %i nodes\n", mNodes.size());
}

void BVH::Draw()
{
	PROFILE_SCOPE(BVH::Draw)

	static Array<DrawData> sDrawDatas;
	sDrawDatas.clear();

	uint64 max_depth = 0;
	GetDrawDataRecursively(sDrawDatas, 0, 0, max_depth);

	for (DrawData& draw_data : sDrawDatas)
	{
		AABB& aabb = draw_data.mAABB;
		fm::vec4 color = {1.f, 0.f, 0.f, 1.f};
		color.y = SCast<float>(draw_data.mDepth) / SCast<float>(max_depth);
		if (draw_data.mLeaf)
			color = {0.f, 1.f, 0.f, 1.f};

		gDrawAABB(aabb, color);
	}
}

void BVH::RefreshObject(const CollisionObjectHandle& inObject)
{
	PROFILE_SCOPE(BVH::RefreshObject)

	const CollisionObject& object = mCollisionWorld->GetCollisionObject(inObject);
	// Early out if the AABB is still fully inside of the encapsulating aabb, which is larger on purpose.
	if (gFullyInsideOf(object.GetAABB(), mObjects[inObject.mIndex].mAABB))
		return;

	AABB aabb = object.GetAABB();
	if (object.GetType() == CollisionObject::EType::Dynamic)
		AdjustAABBForDynamicObject(aabb);

	// If this BVH has not been generated yet there are no nodes to resize, so we can early out.
	if (!IsGenerated())
	{
		mObjects[inObject.mIndex].mAABB = aabb;
		return;
	}

	const Array<uint64>& node_hierarchy = FindNodeHierarchyContainingObject(inObject);

	gAssert(mObjects[mIndices[node_hierarchy[0]]].mCollisionObjectHandle == inObject, "Object not found in BVH!");
	gAssert(node_hierarchy.size() > 1, "Object not found in BVH!");

	mObjects[mIndices[node_hierarchy[0]]].mAABB = aabb;

	// Here we resize the nodes to tighly fit around their objects
	// Index 1 is the leaf node
	BVHNode* leaf_node = &mNodes[node_hierarchy[1]];
	leaf_node->mAABB = CreateAABBFromObjects(leaf_node->mLeftFirst, leaf_node->mCount);

	// Here we don't need to calculate the node to encompass all objects, only the other node's AABBs
	// Starting at 1 since we don't want to handle the index of the object itself, only the nodes for resizing
	for (uint64 i = 2; i < node_hierarchy.size(); i++)
	{
		BVHNode* node = &mNodes[node_hierarchy[i]];
		node->mAABB = gComputeEncompassingAABB(mNodes[node->mLeftFirst].mAABB, mNodes[node->mLeftFirst + 1].mAABB);
	}
}

const Array<CollisionObjectHandle>& BVH::GetBroadphaseCollisions(const AABB& inAABB)
{
	PROFILE_SCOPE(BVH::GetBroadphaseCollisions)

	static THREADLOCAL Array<CollisionObjectHandle> sReturnArray;
	sReturnArray.clear();

	ScopedMutexReadLock lock(mBVHMutex);
	CollisionInternal(sReturnArray, inAABB, 0);

	return sReturnArray;
}

AABB BVH::CreateAABBFromObjects(uint64 inFirst, uint64 inCount)
{
	fm::vec2 min = {INFINITY};
	fm::vec2 max = {-INFINITY};

	for (uint32 i = 0; i < inCount; i++)
	{
		const AABB& mObjectAABB = mObjects[mIndices[inFirst + i]].mAABB;
		min.x = fm::min(mObjectAABB.mMin.x, min.x);
		min.y = fm::min(mObjectAABB.mMin.y, min.y);

		max.x = fm::max(mObjectAABB.mMax.x, max.x);
		max.y = fm::max(mObjectAABB.mMax.y, max.y);
	}

	return {min, max};
}

void BVH::Subdivide(BVHNode* inNode, uint64 inFirst, uint64 inCount, uint64 inDepth)
{
	PROFILE_SCOPE(BVH::Subdivide)

	inNode->mAABB = CreateAABBFromObjects(inFirst, inCount);
	Partition(inNode, inFirst, inCount, inDepth);
}

void BVH::Partition(BVHNode* inNode, uint64 inFirst, uint64 inCount, uint64 inDepth)
{
	fm::vec2 sides_length = inNode->mAABB.mMax - inNode->mAABB.mMin;
	float best_split_location = 0;
	float best_split_cost = INFINITY;
	size_t best_split_axis = 0;

	uint64 split_amount = 20;
	if (inCount - 1 < split_amount)
		split_amount = inCount - 1;
	float split_amount_flt = static_cast<float>(split_amount);
	float split_amount_inv_flt = 1.f / split_amount_flt;

	for (size_t axis = 0; axis < 2; axis++)
	{
		float next_val = 0;
		for (size_t i = 0; i < split_amount; i++)
		{
			next_val += sides_length[axis] * split_amount_inv_flt;
			float split_cost = ComputeSplitCost(inFirst, inCount, inNode->mAABB.mMin[axis] + next_val, axis);
			if (split_cost < best_split_cost)
			{
				best_split_cost = split_cost;
				best_split_location = inNode->mAABB.mMin[axis] + next_val;
				best_split_axis = axis;
			}
		}
	}

	//Leaf node
	if (best_split_cost >= inNode->mAABB.Area() * SCast<float>(inCount))
	{
		inNode->mLeftFirst = SCast<uint32>(inFirst);
		inNode->mCount = SCast<uint32>(inCount);
		return;
	}

	uint64 index = SplitIndices(inFirst, inCount, best_split_location, best_split_axis);
	uint64 left_count = index - inFirst;

	inNode->mLeftFirst = SCast<uint32>(mNodes.size());

	//Children
	mNodes.emplace_back(BVHNode());
	mNodes.emplace_back(BVHNode());

	Subdivide(&mNodes[inNode->mLeftFirst], inFirst, left_count, inDepth + 1);
	Subdivide(&mNodes[inNode->mLeftFirst + 1], index, inCount - left_count, inDepth + 1);
}

float BVH::ComputeSplitCost(uint64 inFirst, uint64 inCount, float inSplitLocation, uint64 inSplitAxis)
{
	uint64 index = SplitIndices(inFirst, inCount, inSplitLocation, inSplitAxis);

	uint64 count_left = index - inFirst;
	uint64 count_right = inCount - count_left;

	AABB aabb_left = CreateAABBFromObjects(inFirst, count_left);
	AABB aabb_right = CreateAABBFromObjects(index, count_right);

	return aabb_left.Area() * count_left + aabb_right.Area() + count_right;
}

uint64 BVH::SplitIndices(uint64 inFirst, uint64 inCount, float inSplitLocation, uint64 inSplitAxis)
{
	uint64 index = inFirst;
	for (uint64 i = inFirst; i < inFirst + inCount; i++)
	{
		const fm::vec2& position = mObjects[mIndices[i]].mAABB.GetCenter();
		if (position[inSplitAxis] <= inSplitLocation)
		{
			//std::swap(m_indices[index], m_indices[i]);
			uint32 t = mIndices[index];
			mIndices[index] = mIndices[i];
			mIndices[i] = t;
			index++;
		}
	}
	return index;
}

void BVH::CollisionInternal(Array<CollisionObjectHandle>& ioReturnArray, const AABB& inAABB, uint64 inNodeIndex)
{
	BVHNode* node = &mNodes[inNodeIndex];
	// Early out if this node does not collide with the aabbb
	if (!gCollides(inAABB, node->mAABB))
		return;

	if (node->mCount != 0) //Leaf node
	{
		// This leaf node collides, add all objects inside of it to the return array.
		for (uint64 i = 0; i < node->mCount; i++)
		{
			const CollisionObjectData& data = mObjects[mIndices[node->mLeftFirst + i]];
			// TODO: Performance profile whether it might be faster to remove this if statement.
			// If it basically always returns true it might.
			if (gCollides(inAABB, data.mAABB))
				ioReturnArray.emplace_back(data.mCollisionObjectHandle);
		}
	}
	else // Branch Node
	{
		CollisionInternal(ioReturnArray, inAABB, node->mLeftFirst);
		CollisionInternal(ioReturnArray, inAABB, node->mLeftFirst + 1);
	}
}

void BVH::AdjustAABBForDynamicObject(AABB& ioAABB)
{
	fm::vec2 size = ioAABB.mMax - ioAABB.mMin;
	ioAABB.mMin -= size * mDynamicAABBModifier;
	ioAABB.mMax += size * mDynamicAABBModifier;
}

// This will find the node hierarchy containing inObject.
// It will be reserved, so index[0] will be the mIndices index of the object itself
// And index[1] will be the mNodes index of the leaf node
// And index.back() will be the root node, 0.
const Array<uint64>& BVH::FindNodeHierarchyContainingObject(const CollisionObjectHandle& inObject)
{
	static THREADLOCAL Array<uint64> sReturnArray;
	sReturnArray.clear();

	ScopedMutexReadLock lock(mBVHMutex);
	fm::vec2 center = mObjects[inObject.mIndex].mAABB.GetCenter();

	FindNodeHierarchyContainingObjectRecursive(sReturnArray, inObject, center, 0);
	sReturnArray.emplace_back(0);

	return sReturnArray;
}

bool BVH::FindNodeHierarchyContainingObjectRecursive(Array<uint64>& ioIndices, const CollisionObjectHandle& inObject, const fm::vec2 inCenter,
													uint64 inNodeIndex)
{
	BVHNode* node = &mNodes[inNodeIndex];
	// Early out if this node does not collide with the aabbb
	if (!gCollides(inCenter, node->mAABB))
		return false;

	if (node->mCount != 0) //Leaf node
	{
		// This leaf node collides, add all objects inside of it to the return array.
		for (uint64 i = 0; i < node->mCount; i++)
		{
			if (mObjects[mIndices[node->mLeftFirst + i]].mCollisionObjectHandle == inObject)
			{
				ioIndices.emplace_back(node->mLeftFirst + i);
				return true;
			}
		}
	}
	else // Branch Node
	{
		if (FindNodeHierarchyContainingObjectRecursive(ioIndices, inObject, inCenter, node->mLeftFirst))
		{
			ioIndices.emplace_back(node->mLeftFirst);
			return true;
		}
		// We can use else if here because only 1 node contains the object, so we can early out
		if (FindNodeHierarchyContainingObjectRecursive(ioIndices, inObject, inCenter, node->mLeftFirst + 1))
		{
			ioIndices.emplace_back(node->mLeftFirst + 1);
			return true;
		}
	}
	return false;
}


// This will find the node hierarchy containing inObject.
// It will be reserved, so index[0] will be the mNodes index of the leaf node
// And index.back() will be the root node, 0.
const Array<uint64>& BVH::FindFirstNodeHierarchyAtLocation(const fm::vec2& inlocation)
{
	static THREADLOCAL Array<uint64> sReturnArray;
	sReturnArray.clear();

	ScopedMutexReadLock lock(mBVHMutex);

	FindFirstNodeHierarchyAtLocationRecursive(sReturnArray, inlocation, 0);
	sReturnArray.emplace_back(0);

	return sReturnArray;
}

bool BVH::FindFirstNodeHierarchyAtLocationRecursive(Array<uint64>& ioIndices, const fm::vec2 inLocation, uint64 inNodeIndex)
{
	BVHNode* node = &mNodes[inNodeIndex];
	// Early out if this node does not collide with the aabbb
	if (!gCollides(inLocation, node->mAABB))
		return false;

	if (node->mCount != 0) //Leaf node
	{
		return true;
	}
	// Branch Node
	if (FindFirstNodeHierarchyAtLocationRecursive(ioIndices, inLocation, node->mLeftFirst))
	{
		ioIndices.emplace_back(node->mLeftFirst);
		return true;
	}
	// We can use else if here because only 1 node contains the object, so we can early out
	if (FindFirstNodeHierarchyAtLocationRecursive(ioIndices, inLocation, node->mLeftFirst + 1))
	{
		ioIndices.emplace_back(node->mLeftFirst + 1);
		return true;
	}
	return false;
}

void BVH::ExpandNodeToFitAABB(BVHNode* ioNode, const AABB& inAABB)
{
	ioNode->mAABB.mMin = min2(ioNode->mAABB.mMin, inAABB.mMin);
	ioNode->mAABB.mMax = max2(ioNode->mAABB.mMax, inAABB.mMax);
}

void BVH::GetDrawDataRecursively(Array<DrawData>& ioDrawData, uint64 inNodeIndex, uint64 inDepth, uint64& ioMaxDepth)
{
	ioMaxDepth = fm::max(inDepth, ioMaxDepth);
	BVHNode* node = &mNodes[inNodeIndex];

	{
		DrawData draw_data;
		draw_data.mAABB = node->mAABB;
		draw_data.mDepth = inDepth;
		ioDrawData.emplace_back(draw_data);
	}

	if (node->mCount != 0) //Leaf node
	{
		// This leaf node collides, add all objects inside of it to the return array.
		for (uint64 i = 0; i < node->mCount; i++)
		{
			DrawData draw_data;
			draw_data.mAABB = mObjects[mIndices[node->mLeftFirst + i]].mAABB;
			draw_data.mDepth = inDepth;
			draw_data.mLeaf = true;
			ioDrawData.emplace_back(draw_data);
		}
	}
	else // Branch Node
	{
		GetDrawDataRecursively(ioDrawData, node->mLeftFirst, inDepth + 1, ioMaxDepth);
		GetDrawDataRecursively(ioDrawData, node->mLeftFirst + 1, inDepth + 1, ioMaxDepth);
	}
}
