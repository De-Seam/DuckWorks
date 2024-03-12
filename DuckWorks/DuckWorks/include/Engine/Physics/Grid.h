#pragma once
#include "Core/CoreBase.h"
#include "Core/Utilities/RTTI.h"

// Engine includes
#include "Engine/World/Chunk.h"

#define TILES_PER_CHUNK_X 20
#define TILES_PER_CHUNK_Y 20
#define TILE_WIDTH (CHUNK_WIDTH / TILES_PER_CHUNK_X)
#define TILE_HEIGHT (CHUNK_HEIGHT / TILES_PER_CHUNK_Y)

class PhysicsObject;

struct GridTile
{
	Array<PhysicsObject*> mObjects;
};

class Grid : public RTTIBaseClass
{
	RTTI_CLASS(Grid, RTTIBaseClass)

	const GridTile& GetTileByIndex(uint64 inIndexX, uint64 inIndexY) const { return mTiles[inIndexX][inIndexY]; }
	const GridTile& GetTileAtLocation(fm::vec2 inLocation) const;

private:

	GridTile mTiles[TILES_PER_CHUNK_X][TILES_PER_CHUNK_Y] = {};
};