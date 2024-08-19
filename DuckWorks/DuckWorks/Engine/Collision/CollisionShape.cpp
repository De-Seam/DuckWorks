#include <Precomp.h>
#include <Engine/Collision/CollisionShape.h>

// Engine includes
#include <Core/Threads/Thread.h>

#include <Engine/Collision/Grid.h>

// Projects a rectangle's corner points onto an axis and returns the scalar projection's min and max
std::pair<float, float> gProjectRectangle(const Rectangle& rect, const Vec2& axis)
{
	float rad = gToRadians(rect.mRotation);
	float cos = std::cos(rad);
	float sin = std::sin(rad);

	// Compute the rectangle's corners
	std::vector<Vec2> corners(4);
	corners[0] = rect.mPosition + Vec2(cos * -rect.mHalfSize.mX - sin * -rect.mHalfSize.mY, sin * -rect.mHalfSize.mX + cos * -rect.mHalfSize.mY);
	corners[1] = rect.mPosition + Vec2(cos * rect.mHalfSize.mX - sin * -rect.mHalfSize.mY, sin * rect.mHalfSize.mX + cos * -rect.mHalfSize.mY);
	corners[2] = rect.mPosition + Vec2(cos * rect.mHalfSize.mX - sin * rect.mHalfSize.mY, sin * rect.mHalfSize.mX + cos * rect.mHalfSize.mY);
	corners[3] = rect.mPosition + Vec2(cos * -rect.mHalfSize.mX - sin * rect.mHalfSize.mY, sin * -rect.mHalfSize.mX + cos * rect.mHalfSize.mY);

	// Initial min and max projection values
	float min = axis.Dot(corners[0]);
	float max = min;
	for (int i = 1; i < 4; ++i)
	{
		float projection = axis.Dot(corners[i]);
		if (projection < min) min = projection;
		if (projection > max) max = projection;
	}
	return {min, max};
}

// Check for overlap on a single axis
bool gOverlaps(const std::pair<float, float>& a, const std::pair<float, float>& b)
{
	return a.first <= b.second && b.first <= a.second;
}

CollisionInfo Rectangle::sCollides(const Rectangle& inRectangleA, const Rectangle& inRectangleB)
{
	float minPenetrationDepth = std::numeric_limits<float>::max();
	Vec2 collisionAxis;
	bool collisionDetected = false;

	float radA = gToRadians(inRectangleA.mRotation);
	float radB = gToRadians(inRectangleB.mRotation);

	std::vector<Vec2> axes = {
		{std::cos(radA), std::sin(radA)},
		{std::cos(radA + gPi() / 2), std::sin(radA + gPi() / 2)},
		{std::cos(radB), std::sin(radB)},
		{std::cos(radB + gPi() / 2), std::sin(radB + gPi() / 2)}
	};

	for (const auto& axis : axes)
	{
		auto projA = gProjectRectangle(inRectangleA, axis);
		auto projB = gProjectRectangle(inRectangleB, axis);
		if (!gOverlaps(projA, projB))
		{
			return CollisionInfo(false, Vec2(0, 0), 0); // No collision
		}
		collisionDetected = true;
		// Calculate overlap (penetration depth) on this axis
		float overlap = std::min(projA.second, projB.second) - std::max(projA.first, projB.first);
		if (overlap < minPenetrationDepth)
		{
			minPenetrationDepth = overlap;
			collisionAxis = axis;
		}
	}

	if (collisionDetected)
	{
		Vec2 centerA = inRectangleA.mPosition;
		Vec2 centerB = inRectangleB.mPosition;
		Vec2 direction = centerB - centerA;

		// Refine collisionAxis based on the direction of the collision
		if (direction.Dot(collisionAxis) > 0)
		{
			collisionAxis = -collisionAxis;
		}
		return CollisionInfo(true, collisionAxis, minPenetrationDepth);
	}
	return CollisionInfo(false, Vec2(0, 0), 0); // Just in case
}

// Helper function to rotate a point around another point by a given angle
Vec2 gRotatePoint(const Vec2& point, const Vec2& origin, float angle)
{
	float cosAngle = std::cos(angle);
	float sinAngle = std::sin(angle);
	Vec2 translatedPoint = point - origin;
	return Vec2{
		cosAngle * translatedPoint.mX - sinAngle * translatedPoint.mY,
		sinAngle * translatedPoint.mX + cosAngle * translatedPoint.mY
	} + origin;
}

Rectangle Rectangle::sComputeSweptShape(const Rectangle& inRectangleA, const Rectangle& inRectangleB)
{
	// Assuming rotation is about the center of the rectangle for both transformations
	Vec2 originalCorners[4] = {
		{inRectangleA.mHalfSize.mX, inRectangleA.mHalfSize.mY},
		{-inRectangleA.mHalfSize.mX, inRectangleA.mHalfSize.mY},
		{-inRectangleA.mHalfSize.mX, -inRectangleA.mHalfSize.mY},
		{inRectangleA.mHalfSize.mX, -inRectangleA.mHalfSize.mY}
	};

	Vec2 newCorners[4] = {
		{inRectangleB.mHalfSize.mX, inRectangleB.mHalfSize.mY},
		{-inRectangleB.mHalfSize.mX, inRectangleB.mHalfSize.mY},
		{-inRectangleB.mHalfSize.mX, -inRectangleB.mHalfSize.mY},
		{inRectangleB.mHalfSize.mX, -inRectangleB.mHalfSize.mY}
	};

	Vec2 allPoints[8];
	// Calculate transformed corner points for both original and new positions
	for (int i = 0; i < 4; ++i)
	{
		allPoints[i] = gRotatePoint(originalCorners[i] + inRectangleA.mPosition, inRectangleA.mPosition, inRectangleA.mRotation);
		allPoints[i + 4] = gRotatePoint(newCorners[i] + inRectangleB.mPosition, inRectangleB.mPosition, inRectangleB.mRotation);
	}

	// Use axis-aligned bounding box (AABB) to approximate the swept shape.
	Vec2 minPoint = allPoints[0], maxPoint = allPoints[0];
	for (const auto& point : allPoints)
	{
		minPoint.mX = std::min(minPoint.mX, point.mX);
		minPoint.mY = std::min(minPoint.mY, point.mY);
		maxPoint.mX = std::max(maxPoint.mX, point.mX);
		maxPoint.mY = std::max(maxPoint.mY, point.mY);
	}

	Vec2 center = (maxPoint + minPoint) * 0.5f;
	Vec2 size = maxPoint - minPoint;

	// Approximate the swept shape's rotation as the mid-point of the original and new rotations
	float midRotation = (inRectangleA.mRotation + inRectangleB.mRotation) / 2.0f;

	return Rectangle{center, size * 0.5f, midRotation};
}

AABB Rectangle::sComputeAABB(const Rectangle& inRectangle)
{
	float rotation = gToRadians(inRectangle.mRotation);
	float sin_r = std::abs(std::sinf(rotation));
	float cos_r = std::abs(std::cosf(rotation));

	// Compute the half-size of the rotated object
	float half_width = inRectangle.mHalfSize.mX * cos_r + inRectangle.mHalfSize.mY * sin_r;
	float half_height = inRectangle.mHalfSize.mX * sin_r + inRectangle.mHalfSize.mY * cos_r;
	Vec2 half_size = {half_width, half_height};

	AABB aabb;
	aabb.mMin = inRectangle.mPosition - half_size;
	aabb.mMax = inRectangle.mPosition + half_size;
	return aabb;
}

void CollisionShape::SetRectangle(Rectangle& ioRectangle)
{
	Rectangle swept_rectangle = Rectangle::sComputeSweptShape(GetRectangle(), ioRectangle);
	AABB swept_aabb = Rectangle::sComputeAABB(swept_rectangle);
	THREADLOCAL static Array<CollisionShape*> sCollisionShapes;
	sCollisionShapes.clear();
	mGrid->GetCollisionShapesInAABB(swept_aabb, sCollisionShapes);

	for (CollisionShape* collision_shape : sCollisionShapes)
	{
		if (collision_shape == this)
			continue;

		CollisionInfo collision_info = Rectangle::sCollides(swept_rectangle, collision_shape->GetRectangle());
		if (!collision_info.mCollides)
			continue;

		ioRectangle.mPosition += collision_info.mDirection * collision_info.mDepth;
		swept_rectangle = Rectangle::sComputeSweptShape(GetRectangle(), ioRectangle);
	}
	mRectangle = ioRectangle;

	OnTransformChanged();
}

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

bool CollisionShape::Collides(const CollisionShape& inOther) const
{
	const Rectangle& other_rect = inOther.GetRectangle();
	return Rectangle::sCollides(mRectangle, other_rect).mCollides;
}

void CollisionShape::OnTransformChanged()
{
	RemoveFromGrid();

	CalculateAABB();

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

void CollisionShape::CalculateAABB()
{
	mAABB = Rectangle::sComputeAABB(mRectangle);
}
