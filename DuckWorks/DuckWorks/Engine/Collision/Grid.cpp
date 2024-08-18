#include <Precomp.h>
#include <Engine/Collision/Grid.h>

// Engine includes
#include <Engine/Engine.h>

// External includes
#include <External/SFML/Graphics/RectangleShape.hpp>

void Grid::Render()
{
	PROFILE_SCOPE(Grid::Render)

	if (mLines.getVertexCount() == 0)
	{
		sf::Vertex vertex;
		vertex.color = sf::Color(150, 150, 150, 100);
		for (int x = 0; x < mTileCount.mX; x++)
		{
			for (int y = 0; y < mTileCount.mY; y++)
			{
				Vec2 min = mMinFlt + IVec2{ x, y }.ToVec2() * mTileSizeFlt;
				Vec2 max = min + mTileSizeFlt;

				vertex.position = min;
				mLines.append(vertex);
				vertex.position = Vec2(min.mX, max.mY);
				mLines.append(vertex);

				vertex.position = Vec2(min.mX, max.mY);
				mLines.append(vertex);
				vertex.position = max;
				mLines.append(vertex);
			}
		}
	}

	gEngine->GetRenderer().Draw(mLines);
}

AABB Grid::GetTileAABB(int inX, int inY) const 
{
	gAssert(IsValidTile(inX, inY));

	AABB aabb;
	aabb.mMin = mMinFlt + IVec2{ inX, inY }.ToVec2() * mTileSizeFlt;
	aabb.mMax = mMinFlt + aabb.mMin + mTileSizeFlt;

	return aabb;
}

void Grid::GetCollisionShapesInAABB(const AABB& inAABB, Array<CollisionShape*>& outCollisionShapes) const
{
	PROFILE_SCOPE(Grid::GetCollisionShapesInAABB)

	IVec2 min_tile = GetTileIndex(inAABB.mMin);
	IVec2 max_tile = GetTileIndex(inAABB.mMax);

	for (int y = min_tile.mY; y <= max_tile.mY; y++)
	{
		for (int x = min_tile.mX; x <= max_tile.mX; x++)
		{
			const Tile& tile = GetTile(x, y);
			for (CollisionShape* collision_shape : tile.mCollisionShapes)
			{
				// Skip duplicates
				if (std::ranges::find(outCollisionShapes, collision_shape) != outCollisionShapes.end())
					continue;

				if (collision_shape->GetAABB().mMin.mX < inAABB.mMax.mX &&
					collision_shape->GetAABB().mMax.mX > inAABB.mMin.mX &&
					collision_shape->GetAABB().mMin.mY < inAABB.mMax.mY &&
					collision_shape->GetAABB().mMax.mY > inAABB.mMin.mY)
				{
					outCollisionShapes.push_back(collision_shape);
				}
			}
		}
	}
}
