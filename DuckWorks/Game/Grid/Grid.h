#pragma once
#include <DuckCore/Containers/Array.h>
#include <DuckCore/Math/Vector.h>
#include <DuckCore/RTTI/Ref.h>
#include <Game/Entity/Entity.h>

struct GridTile
{
	DC::WeakRef<Entity> mEntity = nullptr; // Entity occupying the grid tile
};

class Grid
{
public:
	explicit Grid(DC::UVec2 inSize);
	explicit Grid(uint inWidth, uint inHeight) : Grid(DC::UVec2(inWidth, inHeight)) {}

	DC::Ref<Entity> GetEntityInTile(const DC::UVec2& inTilePosition);

	int GetTileIndex(const DC::UVec2& inTilePosition) const;
	int GetTileIndex(uint inTileX, uint inTileY) const;
	GridTile& GetTile(const DC::UVec2& inTilePosition);
	GridTile& GetTile(uint inTileX, uint inTileY);
	const GridTile& GetTile(const DC::UVec2& inTilePosition) const;
	const GridTile& GetTile(uint inTileX, uint inTileY) const;

private:
	// [width * height] amount of tiles 
	DC::Array<GridTile> mTiles;
	DC::UVec2 mSize;
};
