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
		vertex.color = sf::Color(200, 200, 200, 150);
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

	//for (int i = 0; i < mTileCount.mX * mTileCount.mY; i++)
	//{
	//	int x = i % mTileCount.mX;
	//	int y = i / mTileCount.mX;
	//
	//	sf::RectangleShape rectangle_shape;
	//	Vec2 position = mMinFlt + IVec2{ x, y }.ToVec2() * mTileSizeFlt;
	//	rectangle_shape.setPosition(position);
	//	rectangle_shape.setSize(mTileSizeFlt);
	//	rectangle_shape.setOutlineColor(sf::Color(255, 255, 255, 255));
	//	rectangle_shape.setOutlineThickness(0.5f);
	//	rectangle_shape.setFillColor(sf::Color(0, 0, 0, 0));
	//	gEngine->GetRenderer().Draw(rectangle_shape);
	//}
}

AABB Grid::GetTileAABB(int inX, int inY) const 
{
	gAssert(IsValidTile(inX, inY));

	AABB aabb;
	aabb.mMin = mMinFlt + IVec2{ inX, inY }.ToVec2() * mTileSizeFlt;
	aabb.mMax = mMinFlt + aabb.mMin + mTileSizeFlt;

	return aabb;
}
