#pragma once
#include <DuckCore/Containers/Array.h>
#include <DuckCore/Math/Vector.h>
#include <DuckCore/RTTI/Ref.h>
#include <Game/Entity/Entity.h>

class GridTile
{
public:
	void SetEntity(Entity& inEntity);

	[[nodiscard]]
	DC::Ref<Entity> GetEntity() { return mEntity.IsAlive() ? mEntity.Ref() : nullptr; }
	entt::entity GetEntityHandle() const { return mEntityHandle; }

private:
	DC::WeakRef<Entity> mEntity = nullptr; // Entity occupying the grid tile
	entt::entity mEntityHandle = entt::null; // Entity handle for the entity occupying the grid tile, or null for none
};

class Grid
{
public:
	explicit Grid(DC::UVec2 inSize);
	explicit Grid(uint inWidth, uint inHeight) : Grid(DC::UVec2(inWidth, inHeight)) {}

	[[nodiscard]]
	DC::Ref<Entity> GetEntityInTile(const DC::UVec2& inTilePosition);

	int GetTileIndex(const DC::UVec2& inTilePosition) const;
	int GetTileIndex(uint inTileX, uint inTileY) const;
	GridTile& GetTile(const DC::UVec2& inTilePosition);
	GridTile& GetTile(uint inTileX, uint inTileY);
	const GridTile& GetTile(const DC::UVec2& inTilePosition) const;
	const GridTile& GetTile(uint inTileX, uint inTileY) const;

	[[nodiscard]]
	const DC::UVec2& GetSize() const { return mSize; }

private:
	// [width * height] amount of tiles 
	DC::Array<GridTile> mTiles;
	DC::UVec2 mSize;
};
