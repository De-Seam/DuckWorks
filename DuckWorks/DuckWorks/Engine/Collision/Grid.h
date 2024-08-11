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
	
};

class Grid
{
public:
	Grid(const IVec2& inSize)
		: mSize(inSize)
	{
		mTiles.resize(inSize.mX * inSize.mY);
	}

	Tile& GetTile(int inX, int inY) { return mTiles[inY * mSize.mX + inX]; }
	const IVec2& GetSize() const { return mSize; }

private:
	Array<Tile> mTiles;

	IVec2 mSize;
};