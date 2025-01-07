#include <Game/Grid/Grid.h>

using namespace DC;

void GridTile::SetEntity(Entity& inEntity)
{
	mEntity = &inEntity;
	mEntityHandle = inEntity.GetEntityHandle();
}

Grid::Grid(UVec2 inSize) :
	mSize(gMove(inSize))
{
	uint grid_tile_count = mSize.mX * mSize.mY;
	gAssert(grid_tile_count > 0);
	mTiles.Resize(grid_tile_count);
}

Ref<Entity> Grid::GetEntityInTile(const UVec2& inTilePosition)
{
	GetTile(inTilePosition).GetEntity();
}

int Grid::GetTileIndex(const UVec2& inTilePosition) const { return inTilePosition.mX + inTilePosition.mY * mSize.mX; }
int Grid::GetTileIndex(uint inTileX, uint inTileY) const { return GetTileIndex({ inTileX, inTileY }); }

GridTile& Grid::GetTile(const UVec2& inTilePosition) { return mTiles[GetTileIndex(inTilePosition)]; }
GridTile& Grid::GetTile(uint inTileX, uint inTileY) { return mTiles[GetTileIndex(inTileX, inTileY)]; }

const GridTile& Grid::GetTile(const UVec2& inTilePosition) const { return mTiles[GetTileIndex(inTilePosition)]; }
const GridTile& Grid::GetTile(uint inTileX, uint inTileY) const { return mTiles[GetTileIndex(inTileX, inTileY)]; }
