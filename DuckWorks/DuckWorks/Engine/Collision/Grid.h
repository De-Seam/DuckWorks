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

	IF_DEBUG( bool mIsInGrid = false;)
};

struct Tile
{
	Array<CollisionShape*> mCollisionShapes;
};

class Grid
{
public:
	Grid(const IVec2& inMin, const IVec2& inSize, const IVec2& inTileSize)
		: mMin(inMin), mSize(inSize), mTileSize(inTileSize)
	{
		mMinFlt = mMin.ToVec2();
		mSizeFlt = mSize.ToVec2();
		mTileSizeFlt = mTileSize.ToVec2();
		mTiles = static_cast<Tile*>(operator new[](sizeof(Tile) * inSize.mX * inSize.mY, std::align_val_t{8}));
	}
	~Grid()
	{
		delete[] mTiles;
	}

	bool IsValidTile(int inX, int inY) const { return inX >= 0 && inX < mSize.mX && inY >= 0 && inY < mSize.mY; }

	Tile& GetTile(int inX, int inY) { gAssert(IsValidTile(inX, inY)); return mTiles[inY * mSize.mX + inX]; }
	const Tile& GetTile(int inX, int inY) const { gAssert(IsValidTile(inX, inY)); return mTiles[inY * mSize.mX + inX]; }

	IVec2 GetTileIndex(const Vec2& inPosition) const { return ((inPosition - mMinFlt) / mTileSizeFlt).ToIVec2(); }

	const IVec2& GetSize() const { return mSize; }

	AABB GetTileAABB(int inX, int inY) const;

private:
	Tile* mTiles;

	IVec2 mMin;
	Vec2 mMinFlt;

	IVec2 mSize;
	Vec2 mSizeFlt;
	
	IVec2 mTileSize;
	Vec2 mTileSizeFlt;
};