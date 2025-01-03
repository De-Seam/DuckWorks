#include <Game/Grid/Grid.h>

using namespace DC;

Grid::Grid(UVec2 inSize) :
	mSize(gMove(inSize))
{
	uint grid_tile_count = mSize.mX * mSize.mY;
	gAssert(grid_tile_count > 0);
	mTiles.Resize(grid_tile_count);
}

Ref<Entity> Grid::GetEntityInTile(const UVec2& inTilePosition)
{
	GridTile& tile = GetTile(inTilePosition);
	if (tile.mEntity == nullptr)
		return nullptr;
	if (!tile.mEntity.IsAlive())
		return nullptr;
	return tile.mEntity.Ref();
}

int Grid::GetTileIndex(const UVec2& inTilePosition) const { return inTilePosition.mX + inTilePosition.mY * mSize.mX; }
int Grid::GetTileIndex(uint inTileX, uint inTileY) const { return GetTileIndex({ inTileX, inTileY }); }

GridTile& Grid::GetTile(const UVec2& inTilePosition) { return mTiles[GetTileIndex(inTilePosition)]; }
GridTile& Grid::GetTile(uint inTileX, uint inTileY) { return mTiles[GetTileIndex(inTileX, inTileY)]; }

const GridTile& Grid::GetTile(const UVec2& inTilePosition) const { return mTiles[GetTileIndex(inTilePosition)]; }
const GridTile& Grid::GetTile(uint inTileX, uint inTileY) const { return mTiles[GetTileIndex(inTileX, inTileY)]; }
