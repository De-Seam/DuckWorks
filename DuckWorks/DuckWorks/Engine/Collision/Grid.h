#pragma once
// Core includes
#include <Core/Containers/Array.h>
#include <Core/Math/Vector.h>
#include <Core/Utilities/Types.h>

class Grid;

struct AABB
{
	Vec2 mMin = { FLT_MAX };
	Vec2 mMax = { FLT_MIN };
};

class CollisionShape
{
public:
	CollisionShape() = default;
	CollisionShape(const Vec2& inPosition, const Vec2& inHalfSize, float inRotation, Grid& inGrid)
		: mPosition(inPosition)
		, mHalfSize(inHalfSize)
		, mRotation(inRotation)
	{
		OnTransformChanged(inGrid);
	}

	void SetPosition(const Vec2& inPosition, Grid& inGrid) { RemoveFromGrid(inGrid); mPosition = inPosition; OnTransformChanged(inGrid); }
	void SetHalfSize(const Vec2& inHalfSize, Grid& inGrid) { RemoveFromGrid(inGrid); mHalfSize = inHalfSize; OnTransformChanged(inGrid); }
	void SetRotation(float inRotation,Grid& inGrid) { RemoveFromGrid(inGrid); mRotation = inRotation; OnTransformChanged(inGrid); }

	void RemoveFromGrid(Grid& inGrid);

private:
	void OnTransformChanged(Grid& inGrid);
	void AddToGrid(Grid& inGrid);

	Vec2 mPosition;
	Vec2 mHalfSize;
	float mRotation = 0.0f;
	AABB mAABB;
};

struct Tile
{
	Array<CollisionShape*> mCollisionShapes;
};

class Grid
{
public:
	Grid(const IVec2& inSize, const IVec2& inTileSize)
		: mSize(inSize), mTileSize(inTileSize)
	{
		mTiles = new (static_cast<std::align_val_t>(8)) Tile[inSize.mX * inSize.mY];
	}
	~Grid()
	{
		delete[] mTiles;
	}

	Tile& GetTile(int inX, int inY) { gAssert(inX >= 0 && inX < mSize.mX && inY >= 0 && inY < mSize.mY); return mTiles[inY * mSize.mX + inX]; }
	const Tile& GetTile(int inX, int inY) const { gAssert(inX >= 0 && inX < mSize.mX && inY >= 0 && inY < mSize.mY); return mTiles[inY * mSize.mX + inX]; }
	const IVec2& GetSize() const { return mSize; }

private:
	Tile* mTiles;

	IVec2 mSize;
	IVec2 mTileSize;
};