#include "Precomp.h"
#include "Engine/Collision/BVH/BVH.h"

#include "Engine/Collision/CollisionWorld.h"

void BVH::AddObject(CollisionObjectHandle inObject)
{
	if(mObjects.size() < inObject.mIndex)
		mObjects.resize(inObject.mIndex + 1);
	else
		gAssert(mObjects[inObject.mIndex].mIndex == inObject.mIndex, "Previously added object somehow has the wrong index!");
	mObjects[inObject.mIndex] = inObject;
}

void BVH::RemoveObject(CollisionObjectHandle inObject)
{
	gAssert(mObjects[inObject.mIndex].mIndex == inObject.mIndex, "Trying to remove an object that was never added!");
	mObjects[inObject.mIndex] = CollisionObjectHandle();
}

void BVH::Generate()
{
	ScopedMutexWriteLock lock(mBVHMutex);

	if (mIndices)
		_aligned_free(mIndices);

	mNodes.clear();

	mIndices = static_cast<unsigned int*>(_aligned_malloc((static_cast<size_t>(mObjects.size()) + 1) * 4, 64));
	for (size_t i = 0; i < mObjects.size(); i++)
		mIndices[i] = static_cast<unsigned int>(i);

	mNodes.reserve(mObjects.size() * 2 + 1);
	mNodes.emplace_back(BVHNode()); //Root Node
	mNodes.emplace_back(BVHNode()); //Empty Node for cache alignment

	mNodes[0].mAABB = mCreateAABBFromObjects(0, mObjects.size());
	Subdivide(&mNodes[0], 0, mObjects.size(), 0);

	gLog("BVH Created with %i nodes\n", mNodes.size());
}

AABB BVH::mCreateAABBFromObjects(uint32 inFirst, uint32 inCount)
{
	fm::vec2 min = { INFINITY };
	fm::vec2 max = { -INFINITY };

	for (uint32 i = 0; i < inCount; i++)
	{
		const AABB& mObjectAABB = mCollisionWorld->GetCollisionObject(mObjects[mIndices[inFirst + i]]).GetAABB();
		min.x = fm::min(mObjectAABB.mMin.x, min.x);
		min.y = fm::min(mObjectAABB.mMin.y, min.y);

		max.x = fm::max(mObjectAABB.mMax.x, max.x);
		max.y = fm::max(mObjectAABB.mMax.y, max.y);
	}

	return { min, max };
}

void BVH::Subdivide(BVHNode* inNode, int32 inFirst, int32 inCount, int32 inDepth)
{
	inNode->mAABB = mCreateAABBFromObjects(inFirst, inFirst + inCount);
	Partition(inNode, inFirst, inCount, inDepth);
}

void BVH::Partition(BVHNode* node, int32 inFirst, int32 inCount, int32 inDepth)
{
	fm::vec2 sides_length = node->mAABB.mMax - node->mAABB.mMin;
	float best_split_location = 0;
	float best_split_cost = INFINITY;
	size_t best_split_axis = 0;

	int split_amount = 20;
	if (inCount < split_amount)
		split_amount = inCount;
	float split_amount_flt = static_cast<float>(split_amount);
	float split_amount_inv_flt = 1.f / split_amount_flt;

	for (size_t axis = 0; axis < 2; axis++)
	{
		float next_val = 0;
		for (size_t i = 0; i < split_amount; i++)
		{
			next_val += sides_length[axis] * split_amount_inv_flt;
			float split_cost = compute_split_cost(node, inFirst, inCount, node->mAABB.mMin[axis] + next_val, axis);
			if (split_cost < best_split_cost)
			{
				best_split_cost = split_cost;
				best_split_location = node->mAABB.mMin[axis] + next_val;
				best_split_axis = axis;
			}
		}
	}

	//Leaf node
	if (best_split_cost >= node->mAABB.area() * static_cast<float>(inCount))
	{
		node->mLeftFirst = inFirst;
		node->mRightCount = inCount;
		return;
	}

	int index = split_indices(node, inFirst, inCount, best_split_location, best_split_axis);
	int left_count = index - inFirst;

	node->mLeftFirst = mNodes.size();

	//Children
	mNodes.emplace_back(BVHNode());
	mNodes.emplace_back(BVHNode());

	subdivide(&m_nodes[node->left_first], inFirst, left_count, inDepth + 1);
	subdivide(&m_nodes[node->left_first + 1], index, inCount - left_count, inDepth + 1);
}