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
	Vec2 mMin, mMax;

	Vec2 GetSidesLength() const
	{
		return mMax - mMin;
	}

	float Area() const
	{
		Vec2 length = GetSidesLength();
		return length.mX * length.mY;
	}

	Vec2 GetCenter() const
	{
		Vec2 length = GetSidesLength();
		return mMax - (length * 0.5f);
	}
};

struct CollisionInfo
{
	bool mCollides;
	Vec2 mDirection;
	float mDepth;

	CollisionInfo(bool c, Vec2 dir, float d) : mCollides(c), mDirection(dir), mDepth(d) {}
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

struct CollisionData
{
	CollisionObjectHandle mHandle;
	CollisionInfo mCollisionInfo;
};
