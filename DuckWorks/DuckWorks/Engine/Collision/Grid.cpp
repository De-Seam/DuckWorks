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
