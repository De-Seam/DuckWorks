#include <Precomp.h>
#include <Engine/Collision/CollisionShape.h>

// Engine includes
#include <Engine/Collision/Grid.h>

void CollisionShape::RemoveFromGrid()
{
	gAssert(mGrid != nullptr);
	gAssert(mIsInGrid);

	IVec2 min_tile = mGrid->GetTileIndex(mAABB.mMin);
	IVec2 max_tile = mGrid->GetTileIndex(mAABB.mMax);

	for (int y = min_tile.mY; y <= max_tile.mY; y++)
	{
		for (int x = min_tile.mX; x <= max_tile.mX; x++)
		{
			Tile& tile = mGrid->GetTile(x, y);
			tile.mCollisionShapes.erase(std::ranges::find(tile.mCollisionShapes, this));
		}
	}

	IF_DEBUG(mIsInGrid = false);
}

void CollisionShape::OnTransformChanged()
{
	RemoveFromGrid();

	// TODO: Included rotation
	mAABB = AABB{ mPosition - mHalfSize, mPosition + mHalfSize };

	AddToGrid();
}

void CollisionShape::AddToGrid()
{
	gAssert(mGrid != nullptr);
	gAssert(!mIsInGrid);

	IVec2 min_tile = mGrid->GetTileIndex(mAABB.mMin);
	IVec2 max_tile = mGrid->GetTileIndex(mAABB.mMax);

	for (int y = min_tile.mY; y <= max_tile.mY; y++)
	{
		for (int x = min_tile.mX; x <= max_tile.mX; x++)
		{
			mGrid->GetTile(x, y).mCollisionShapes.push_back(this);
		}
	}

	IF_DEBUG(mIsInGrid = true);
}