#pragma once
#include "Core/CoreBase.h"

// Each chunk is 10k x 10k
#define CHUNK_WIDTH 10000
#define CHUNK_HEIGHT 10000

#define TILES_PER_CHUNK_X 20
#define TILES_PER_CHUNK_Y 20
#define TILE_WIDTH (CHUNK_WIDTH / TILES_PER_CHUNK_X)
#define TILE_HEIGHT (CHUNK_HEIGHT / TILES_PER_CHUNK_Y)

using CollisionObjectHandle = Handle<struct CollisionObjectHandleEmptyStruct>;

struct AABB
{
	fm::vec2 mMin, mMax;

	fm::vec2 GetSidesLength() const
	{
		return mMax - mMin;
	}

	float Area() const
	{
		fm::vec2 length = GetSidesLength();
		return length.x * length.y;
	}

	fm::vec2 GetCenter() const
	{
		fm::vec2 length = GetSidesLength();
		return mMax - (length * 0.5f);
	}
};

struct CollisionInfo
{
	bool mCollides;
	fm::vec2 mDirection;
	float mDepth;

	CollisionInfo(bool c, fm::vec2 dir, float d) : mCollides(c), mDirection(dir), mDepth(d) {}
	CollisionInfo() = default;
};

struct CollisionFuncParams
{
	CollisionFuncParams() = default;

	CollisionObjectHandle mSelf;
	CollisionObjectHandle mOther;
	CollisionInfo mCollisionInfo;
};

using OnCollisionFunc = std::function<void(const CollisionFuncParams& inParams)>;
