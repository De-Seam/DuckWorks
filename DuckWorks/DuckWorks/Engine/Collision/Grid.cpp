#include <Precomp.h>
#include <Engine/Collision/Grid.h>

void CollisionShape::RemoveFromGrid(Grid& inGrid)
{
	gAssert(mIsInGrid);

	IVec2 min_tile = inGrid.GetTileIndex(mAABB.mMin);
	IVec2 max_tile = inGrid.GetTileIndex(mAABB.mMax);

	for (int y = min_tile.mY; y <= max_tile.mY; ++y)
	{
		for (int x = min_tile.mX; x <= max_tile.mX; ++x)
		{
			Tile& tile = inGrid.GetTile(x, y);
			tile.mCollisionShapes.erase(std::ranges::find(tile.mCollisionShapes, this));
		}
	}

	IF_DEBUG(mIsInGrid = false);
}

void CollisionShape::OnTransformChanged(Grid& inGrid)
{
	RemoveFromGrid(inGrid);

	// TODO: Included rotation
	mAABB = AABB{ mPosition - mHalfSize, mPosition + mHalfSize };

	AddToGrid(inGrid);
}

void CollisionShape::AddToGrid(Grid& inGrid)
{
	gAssert(!mIsInGrid);

	IVec2 min_tile = inGrid.GetTileIndex(mAABB.mMin);
	IVec2 max_tile = inGrid.GetTileIndex(mAABB.mMax);

	for (int y = min_tile.mY; y <= max_tile.mY; ++y)
	{
		for (int x = min_tile.mX; x <= max_tile.mX; ++x)
		{
			inGrid.GetTile(x, y).mCollisionShapes.push_back(this);
		}
	}

	IF_DEBUG(mIsInGrid = true);
}

AABB Grid::GetTileAABB(int inX, int inY) const 
{
	gAssert(IsValidTile(inX, inY));

	AABB aabb;
	aabb.mMin = mMinFlt + IVec2{ inX, inY }.ToVec2() * mTileSizeFlt;
	aabb.mMax = mMinFlt + aabb.mMin + mTileSizeFlt;

	return aabb;
}
