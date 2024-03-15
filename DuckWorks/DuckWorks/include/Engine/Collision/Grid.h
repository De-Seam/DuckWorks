#pragma once
#include "Core/CoreBase.h"
#include "Core/Utilities/RTTI.h"

// Engine includes
#include "CollisionStructs.h"

struct AABB;

class Grid : public RTTIBaseClass
{
	RTTI_CLASS(Grid, RTTIBaseClass)

	struct Tile
	{
		Array<CollisionObjectHandle> mObjects;
	};

	using TileIndex = Pair<uint64, uint64>;

	const Tile& GetTileByIndex(TileIndex index) const { return mTiles[index.first][index.second]; }
	const Tile& GetTileAtLocation(fm::vec2 inLocation) const;
	TileIndex GetTileIndexAtLocation(fm::vec2 inLocation) const;
	Array<TileIndex> GetGridTilesInAABB(const AABB& inAABB) const;

	// These functions are usually called after each other.
	void RemoveObjectFromTiles(const CollisionObjectHandle& inObject, const AABB& inOldAABB);
	void AddObjectToTiles(const CollisionObjectHandle& inObject);

private:
	Tile mTiles[TILES_PER_CHUNK_X][TILES_PER_CHUNK_Y] = {};
};
