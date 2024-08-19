#pragma once
// Core includes
#include <Core/Containers/Array.h>
#include <Core/Math/Vector.h>
#include <Core/Utilities/Types.h>

// Engine includes
#include <Engine/Collision/CollisionShape.h>

// External includes
#include <External/SFML/Graphics/VertexArray.hpp>

struct Tile
{
	Array<CollisionShape*> mCollisionShapes;
};

class Grid
{
public:
	// Size = tile count
	Grid(const IVec2& inMin, const IVec2& inTileCount, const IVec2& inTileSize)
		: mMin(inMin), mTileCount(inTileCount), mTileSize(inTileSize)
	{
		mMinFlt = mMin.ToVec2();
		mTileCountFlt = mTileCount.ToVec2();
		mTileSizeFlt = mTileSize.ToVec2();
		//mTiles = static_cast<Tile*>(operator new[](sizeof(Tile) * inTileCount.mX * inTileCount.mY, std::align_val_t{8}));
		mTiles = new Tile[inTileCount.mX * inTileCount.mY];
	}
	~Grid()
	{
		//operator delete[](mTiles, std::align_val_t{8});
		delete[] mTiles;
	}

	void Render();

	bool IsValidTile(int inX, int inY) const { return inX >= 0 && inX < mTileCount.mX && inY >= 0 && inY < mTileCount.mY; }

	Tile& GetTile(int inX, int inY) { gAssert(IsValidTile(inX, inY)); return mTiles[inY * mTileCount.mX + inX]; }
	const Tile& GetTile(int inX, int inY) const { gAssert(IsValidTile(inX, inY)); return mTiles[inY * mTileCount.mX + inX]; }

	IVec2 GetTileIndex(const Vec2& inPosition) const { return ((inPosition - mMinFlt) / mTileSizeFlt).ToIVec2(); }

	AABB GetTileAABB(int inX, int inY) const;

	const IVec2& GetTileCount() const { return mTileCount; }

	void GetCollisionShapesInAABB(const AABB& inAABB, Array<CollisionShape*>& outCollisionShapes) const;


private:
	// Array of tiles
	Tile* mTiles;

	// Min coords
	IVec2 mMin;
	Vec2 mMinFlt;

	// Amount of tiles
	IVec2 mTileCount;
	Vec2 mTileCountFlt;

	// Size of each tile
	IVec2 mTileSize;
	Vec2 mTileSizeFlt;

	sf::VertexArray mLines = sf::VertexArray(sf::Lines); 
};