#include "Precomp.h"
#include "Engine/Collision/Grid.h"
#include "Engine/Collision/CollisionObject.h"
#include "Engine/Collision/CollisionStructs.h"

const Grid::Tile& Grid::GetTileAtLocation(fm::vec2 inLocation) const
{
	TileIndex tile_index = GetTileIndexAtLocation(inLocation);
	return GetTileByIndex(tile_index);
}

Grid::TileIndex Grid::GetTileIndexAtLocation(fm::vec2 inLocation) const
{
	gAssert(inLocation.x < CHUNK_WIDTH && inLocation.y < CHUNK_HEIGHT, "Out of bounds!");
	fm::ivec2 location = {SCast<int32>(inLocation.x), SCast<int32>(inLocation.y)};
	int32 index_x = location.x / TILE_WIDTH;
	int32 index_y = location.y / TILE_HEIGHT;
	return {index_x, index_y};
}

Array<Grid::TileIndex> Grid::GetGridTilesInAABB(const AABB& inAABB) const
{
	static THREADLOCAL Array<TileIndex> return_array;
	return_array.clear();
	gAssert(return_array.empty(), "Somehow return array wasn't empty at start!");
	TileIndex index_min = GetTileIndexAtLocation(inAABB.mMin);
	TileIndex index_max = GetTileIndexAtLocation(inAABB.mMax);
	for (uint64 x = index_min.first; x <= index_max.first; x++)
	{
		for (uint64 y = index_min.second; y <= index_max.second; y++)
		{
			return_array.emplace_back(TileIndex{x, y});
		}
	}
	return return_array;
}

void Grid::RemoveObjectFromTiles(const CollisionObjectHandle& inObject, const AABB& inOldAABB)
{
	// Static threadlocal here so we don't need to re-alloc memory each time it's called
	static THREADLOCAL Array<TileIndex> tile_indices;
	tile_indices = GetGridTilesInAABB(inOldAABB);
	for (uint64 i = 0; i < tile_indices.size(); i++)
	{
		Tile& tile = mTiles[tile_indices[i].first][tile_indices[i].second];
		for (auto rit = tile.mObjects.rbegin(); rit != tile.mObjects.rend(); ++rit)
		{
			CollisionObjectHandle& object = *rit;
			if (object == inObject)
			{
				std::iter_swap(rit, tile.mObjects.rbegin());
				tile.mObjects.pop_back();
				break;
			}
		}
	}
}

void Grid::AddObjectToTiles(const CollisionObjectHandle& inObject)
{
	const AABB& aabb = {}; //inObject->GetAABB();

	// Static threadlocal here so we don't need to re-alloc memory each time it's caled
	static THREADLOCAL Array<TileIndex> tile_indices;
	tile_indices = GetGridTilesInAABB(aabb);

	for (uint64 i = 0; i < tile_indices.size(); i++)
	{
		Tile& tile = mTiles[tile_indices[i].first][tile_indices[i].second];
		gAssert(std::find(std::begin(tile.mObjects), std::end(tile.mObjects), inObject) == std::end(tile.mObjects), "Object was already in the grid tile!");
		tile.mObjects.emplace_back(inObject);
	}
}