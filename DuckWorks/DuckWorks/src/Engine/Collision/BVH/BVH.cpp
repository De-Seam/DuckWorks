#include "Precomp.h"
#include "Engine/Collision/BVH/BVH.h"

// Engine includes
#include "Engine/Collision/CollisionWorld.h"
#include "Engine/Collision/CollisionHelperFunctions.h"

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

	mIndices = static_cast<uint32*>(_aligned_malloc((static_cast<size_t>(mObjects.size()) + 1) * 4, 64));
	for (size_t i = 0; i < mObjects.size(); i++)
		mIndices[i] = static_cast<uint32>(i);

	mNodes.reserve(mObjects.size() * 2 + 1);
	mNodes.emplace_back(BVHNode()); //Root Node
	mNodes.emplace_back(BVHNode()); //Empty Node for cache alignment

	mNodes[0].mAABB = CreateAABBFromObjects(0, mObjects.size());
	Subdivide(&mNodes[0], 0, mObjects.size(), 0);

	gLog("BVH Created with %i nodes\n", mNodes.size());
}

const Array<CollisionObjectHandle>& BVH::GetBroadphaseCollisions(const AABB& inAABB)
{
	static THREADLOCAL Array<CollisionObjectHandle> sReturnArray;
	sReturnArray.empty();

	ScopedMutexReadLock lock(mBVHMutex);
	CollisionInternal(sReturnArray, inAABB, 0);

	return sReturnArray;
}

AABB BVH::CreateAABBFromObjects(uint64 inFirst, uint64 inCount)
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

void BVH::Subdivide(BVHNode* inNode, uint64 inFirst, uint64 inCount, uint64 inDepth)
{
	inNode->mAABB = CreateAABBFromObjects(inFirst, inFirst + inCount);
	Partition(inNode, inFirst, inCount, inDepth);
}

void BVH::Partition(BVHNode* inNode, uint64 inFirst, uint64 inCount, uint64 inDepth)
{
	fm::vec2 sides_length = inNode->mAABB.mMax - inNode->mAABB.mMin;
	float best_split_location = 0;
	float best_split_cost = INFINITY;
	size_t best_split_axis = 0;

	uint64 split_amount = 20;
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

	AABB aabb_left = CreateAABBFromObjects(inFirst, inFirst + count_left);
	AABB aabb_right = CreateAABBFromObjects(index, index + count_right);

	return aabb_left.Area() * count_left + aabb_right.Area() + count_right;
}

uint64 BVH::SplitIndices(uint64 inFirst, uint64 inCount, float inSplitLocation, uint64 inSplitAxis)
{
	uint64 index = inFirst;
	for (uint64 i = inFirst; i < inFirst + inCount; i++)
	{
		const CollisionObject& object = mCollisionWorld->GetCollisionObject(mObjects[mIndices[i]]);
		const fm::vec2& position = object.GetPosition();
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
			const CollisionObjectHandle& handle = mObjects[mIndices[node->mLeftFirst + i]];
			ioReturnArray.emplace_back(handle);
		}
	}
	else // Branch Node
	{
		CollisionInternal(ioReturnArray, inAABB, node->mLeftFirst);
		CollisionInternal(ioReturnArray, inAABB, node->mLeftFirst+1);
	}
}
