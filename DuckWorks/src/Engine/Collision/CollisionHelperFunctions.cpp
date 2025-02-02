#include "Precomp.h"
#include "Engine/Collision/CollisionHelperFunctions.h"

bool gFullyInsideOf(const AABB& inA, const AABB& inB)
{
	return
		inA.mMin.mX >= inB.mMin.mX &&
		inA.mMax.mX <= inB.mMax.mX &&
		inA.mMin.mY >= inB.mMin.mY &&
		inA.mMax.mY <= inB.mMax.mY;
}

bool gCollides(const AABB& inA, const AABB& inB)
{
	return
		inA.mMax.mX >= inB.mMin.mX &&
		inA.mMin.mX <= inB.mMax.mX &&
		inA.mMax.mY >= inB.mMin.mY &&
		inA.mMin.mY <= inB.mMax.mY;
}

bool gCollides(const Vec2& inPoint, const AABB& inAABB)
{
	return
		inPoint.mX >= inAABB.mMin.mX &&
		inPoint.mX <= inAABB.mMax.mX &&
		inPoint.mY >= inAABB.mMin.mY &&
		inPoint.mY <= inAABB.mMax.mY;
}

bool gCollides(const Vec2& inPoint, const Transform2D& inTransform)
{
	float rad = gToRadians(inTransform.mRotation);
	float cos = std::cos(rad);
	float sin = std::sin(rad);

	// Translate point to the origin of the rectangle
	Vec2 local_point = inPoint - inTransform.mPosition;

	// Rotate the point by the negative rotation angle of the rectangle
	float local_x = local_point.mX * cos + local_point.mY * sin;
	float local_y = -local_point.mX * sin + local_point.mY * cos;

	// Check if the point is within the bounds of the rectangle
	return std::abs(local_x) <= inTransform.mHalfSize.mX && std::abs(local_y) <= inTransform.mHalfSize.mY;
}

// Projects a rectangle's corner points onto an axis and returns the scalar projection's min and max
std::pair<float, float> gProjectRectangle(const Transform2D& rect, const Vec2& axis)
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

CollisionInfo gCollides(const Transform2D& inTransformA, const Transform2D& inTransformB)
{
	float minPenetrationDepth = std::numeric_limits<float>::max();
	Vec2 collisionAxis;
	bool collisionDetected = false;

	float radA = gToRadians(inTransformA.mRotation);
	float radB = gToRadians(inTransformB.mRotation);

	std::vector<Vec2> axes = {
		{std::cos(radA), std::sin(radA)},
		{std::cos(radA + gPi() / 2), std::sin(radA + gPi() / 2)},
		{std::cos(radB), std::sin(radB)},
		{std::cos(radB + gPi() / 2), std::sin(radB + gPi() / 2)}
	};

	for (const auto& axis : axes)
	{
		auto projA = gProjectRectangle(inTransformA, axis);
		auto projB = gProjectRectangle(inTransformB, axis);
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
		Vec2 centerA = inTransformA.mPosition;
		Vec2 centerB = inTransformB.mPosition;
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
Vec2 RotatePoint(const Vec2& point, const Vec2& origin, float angle)
{
	float cosAngle = std::cos(angle);
	float sinAngle = std::sin(angle);
	Vec2 translatedPoint = point - origin;
	return Vec2{
		cosAngle * translatedPoint.mX - sinAngle * translatedPoint.mY,
		sinAngle * translatedPoint.mX + cosAngle * translatedPoint.mY
	} + origin;
}

Transform2D gComputeSweptShape(const Transform2D& inOriginalTransform, const Vec2& inNewPosition, float inNewRotation)
{
	// Assuming rotation is about the center of the rectangle
	Vec2 corners[4] = {
		{inOriginalTransform.mHalfSize.mX, inOriginalTransform.mHalfSize.mY},
		{-inOriginalTransform.mHalfSize.mX, inOriginalTransform.mHalfSize.mY},
		{-inOriginalTransform.mHalfSize.mX, -inOriginalTransform.mHalfSize.mY},
		{inOriginalTransform.mHalfSize.mX, -inOriginalTransform.mHalfSize.mY}
	};

	Vec2 allPoints[8];
	// Calculate original and new corner points
	for (int i = 0; i < 4; ++i)
	{
		allPoints[i] = RotatePoint(corners[i] + inOriginalTransform.mPosition, inOriginalTransform.mPosition, inOriginalTransform.mRotation);
		allPoints[i + 4] = RotatePoint(corners[i] + inNewPosition, inNewPosition, inNewRotation);
	}

	// Simplification: use axis-aligned bounding box of all points to approximate the swept shape
	// and then apply the original rotation to this bounding box for the swept shape.
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

	// Return the approximate swept shape as a Transform2D
	// This approximation uses the mid-point of the original and new rotations for the swept shape's rotation
	float midRotation = (inOriginalTransform.mRotation + inNewRotation) / 2.0f;
	return Transform2D{center, size * 0.5f, midRotation};
}

Transform2D gComputeSweptShape(const Transform2D& inOriginalTransform, const Transform2D& inNewTransform)
{
	// Assuming rotation is about the center of the rectangle for both transformations
	Vec2 originalCorners[4] = {
		{inOriginalTransform.mHalfSize.mX, inOriginalTransform.mHalfSize.mY},
		{-inOriginalTransform.mHalfSize.mX, inOriginalTransform.mHalfSize.mY},
		{-inOriginalTransform.mHalfSize.mX, -inOriginalTransform.mHalfSize.mY},
		{inOriginalTransform.mHalfSize.mX, -inOriginalTransform.mHalfSize.mY}
	};

	Vec2 newCorners[4] = {
		{inNewTransform.mHalfSize.mX, inNewTransform.mHalfSize.mY},
		{-inNewTransform.mHalfSize.mX, inNewTransform.mHalfSize.mY},
		{-inNewTransform.mHalfSize.mX, -inNewTransform.mHalfSize.mY},
		{inNewTransform.mHalfSize.mX, -inNewTransform.mHalfSize.mY}
	};

	Vec2 allPoints[8];
	// Calculate transformed corner points for both original and new positions
	for (int i = 0; i < 4; ++i)
	{
		allPoints[i] = RotatePoint(originalCorners[i] + inOriginalTransform.mPosition, inOriginalTransform.mPosition, inOriginalTransform.mRotation);
		allPoints[i + 4] = RotatePoint(newCorners[i] + inNewTransform.mPosition, inNewTransform.mPosition, inNewTransform.mRotation);
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
	float midRotation = (inOriginalTransform.mRotation + inNewTransform.mRotation) / 2.0f;

	return Transform2D{center, size * 0.5f, midRotation};
}

AABB gComputeAABB(const Transform2D& inTransform)
{
	float rotation = gToRadians(inTransform.mRotation);
	float sin_r = std::abs(std::sinf(rotation));
	float cos_r = std::abs(std::cosf(rotation));

	// Compute the half-size of the rotated object
	float half_width = inTransform.mHalfSize.mX * cos_r + inTransform.mHalfSize.mY * sin_r;
	float half_height = inTransform.mHalfSize.mX * sin_r + inTransform.mHalfSize.mY * cos_r;
	Vec2 half_size = {half_width, half_height};

	AABB aabb;
	aabb.mMin = inTransform.mPosition - half_size;
	aabb.mMax = inTransform.mPosition + half_size;

	return aabb;
}

AABB gComputeEncompassingAABB(const AABB& inA, const AABB& inB)
{
	AABB aabb;
	aabb.mMin = gMin2(inA.mMin, inB.mMin);
	aabb.mMax = gMax2(inA.mMax, inB.mMax);
	return aabb;
}
