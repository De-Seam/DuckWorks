#include <Precomp.h>
#include <Engine/Collision/Grid.h>

void CollisionShape::RemoveFromGrid(Grid& inGrid)
{

}

void CollisionShape::OnTransformChanged(Grid& inGrid)
{
	// TODO: Included rotation
	mAABB = AABB{ mPosition - mHalfSize, mPosition + mHalfSize };

	AddToGrid(inGrid);
}

void CollisionShape::AddToGrid(Grid& inGrid)
{

}
