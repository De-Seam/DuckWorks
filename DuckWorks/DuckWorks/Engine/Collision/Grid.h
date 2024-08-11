#pragma once
// Core includes
#include <Core/Containers/Array.h>
#include <Core/Math/Vector.h>
#include <Core/Utilities/Types.h>

class CollisionShape
{
public:

private:
	Vec2 mPosition;
	Vec2 mHalfSize;
	float mRotation = 0.0f;
};

struct Tile
{
	Array<CollisionShape*> mCollisionShapes;
};

class Grid
{
public:
	Grid(const IVec2& inSize)
		: mSize(inSize)
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
};