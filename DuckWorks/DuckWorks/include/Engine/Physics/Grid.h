#pragma once
#include "Core/CoreBase.h"
#include "Core/Utilities/RTTI.h"

// Engine includes
#include "Engine/World/Chunk.h"
#include <External/rapidjson/document.h>
#include "PhysicStructs.h"

#define TILES_PER_CHUNK_X 20
#define TILES_PER_CHUNK_Y 20
#define TILE_WIDTH (CHUNK_WIDTH / TILES_PER_CHUNK_X)
#define TILE_HEIGHT (CHUNK_HEIGHT / TILES_PER_CHUNK_Y)

class PhysicsObject;
struct AABB;

struct GridTile
{
	Array<PhysicsObject*> mObjects;
};

class Grid : public RTTIBaseClass
{
	RTTI_CLASS(Grid, RTTIBaseClass)

	using TileIndex = Pair<uint64, uint64>;

	const GridTile& GetTileByIndex(TileIndex index) const { return mTiles[index.first][index.second]; }
	const GridTile& GetTileAtLocation(fm::vec2 inLocation) const;
	const TileIndex GetTileIndexAtLocation(fm::vec2 inLocation) const;
	const Array<TileIndex> GetGridTilesInAABB(const AABB& inAABB) const;

	// These functions are usually called after each other.
	void RemoveObjectFromTiles(const PhysicsObject* inObject, const AABB& inOldAABB);
	void AddObjectToTiles(const PhysicsObject* inObject);

private:

	GridTile mTiles[TILES_PER_CHUNK_X][TILES_PER_CHUNK_Y] = {};
};