#include "Precomp.h"
#include "Engine/Physics/PhysicsHelperFunctions.h"

bool gCollides(const AABB& inA, const AABB& inB)
{
	return
		inA.mMax.x >= inB.mMin.x &&
		inA.mMin.x <= inB.mMax.x &&
		inA.mMax.y >= inB.mMin.y &&
		inA.mMin.y <= inB.mMax.y;
}


// Projects a rectangle's corner points onto an axis and returns the scalar projection's min and max
std::pair<float, float> gProjectRectangle(const fm::Transform2D& rect, const fm::vec2& axis)
{
	float rad = fm::to_radians(rect.rotation);
	float cos = std::cos(rad);
	float sin = std::sin(rad);

	// Compute the rectangle's corners
	std::vector<fm::vec2> corners(4);
	corners[0] = rect.position + fm::vec2(cos * -rect.halfSize.x - sin * -rect.halfSize.y, sin * -rect.halfSize.x + cos * -rect.halfSize.y);
	corners[1] = rect.position + fm::vec2(cos * rect.halfSize.x - sin * -rect.halfSize.y, sin * rect.halfSize.x + cos * -rect.halfSize.y);
	corners[2] = rect.position + fm::vec2(cos * rect.halfSize.x - sin * rect.halfSize.y, sin * rect.halfSize.x + cos * rect.halfSize.y);
	corners[3] = rect.position + fm::vec2(cos * -rect.halfSize.x - sin * rect.halfSize.y, sin * -rect.halfSize.x + cos * rect.halfSize.y);

	// Initial min and max projection values
	float min = axis.dot(corners[0]);
	float max = min;
	for (int i = 1; i < 4; ++i)
	{
		float projection = axis.dot(corners[i]);
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

// Main collision detection function using SAT
bool gCollides(const fm::Transform2D& inTransformA, const fm::Transform2D& inTransformB)
{
	float radA = fm::to_radians(inTransformA.rotation);
	float radB = fm::to_radians(inTransformB.rotation);

	// Axes to test against, derived from the edges of both rectangles
	std::vector<fm::vec2> axes = {
		{std::cos(radA), std::sin(radA)}, // A's x-axis
		{std::cos(radA + fm::pi() / 2), std::sin(radA + fm::pi() / 2)}, // A's y-axis
		{std::cos(radB), std::sin(radB)}, // B's x-axis
		{std::cos(radB + fm::pi() / 2), std::sin(radB + fm::pi() / 2)} // B's y-axis
	};

	for (const auto& axis : axes)
	{
		auto projA = gProjectRectangle(inTransformA, axis);
		auto projB = gProjectRectangle(inTransformB, axis);
		if (!gOverlaps(projA, projB))
		{
			return false; // No collision if there's no overlap on this axis
		}
	}

	return true; // Collision detected
}

// Helper function to rotate a point around another point by a given angle
fm::vec2 RotatePoint(const fm::vec2& point, const fm::vec2& origin, float angle)
{
	float cosAngle = std::cos(angle);
	float sinAngle = std::sin(angle);
	fm::vec2 translatedPoint = point - origin;
	return fm::vec2{
		cosAngle * translatedPoint.x - sinAngle * translatedPoint.y,
		sinAngle * translatedPoint.x + cosAngle * translatedPoint.y
	} + origin;
}

fm::Transform2D gComputeSweptShape(const fm::Transform2D& inOriginalTransform, const fm::vec2& inNewPosition, float inNewRotation)
{
	// Assuming rotation is about the center of the rectangle
	fm::vec2 corners[4] = {
		{inOriginalTransform.halfSize.x, inOriginalTransform.halfSize.y},
		{-inOriginalTransform.halfSize.x, inOriginalTransform.halfSize.y},
		{-inOriginalTransform.halfSize.x, -inOriginalTransform.halfSize.y},
		{inOriginalTransform.halfSize.x, -inOriginalTransform.halfSize.y}
	};

	fm::vec2 allPoints[8];
	// Calculate original and new corner points
	for (int i = 0; i < 4; ++i)
	{
		allPoints[i] = RotatePoint(corners[i] + inOriginalTransform.position, inOriginalTransform.position, inOriginalTransform.rotation);
		allPoints[i + 4] = RotatePoint(corners[i] + inNewPosition, inNewPosition, inNewRotation);
	}

	// Simplification: use axis-aligned bounding box of all points to approximate the swept shape
	// and then apply the original rotation to this bounding box for the swept shape.
	fm::vec2 minPoint = allPoints[0], maxPoint = allPoints[0];
	for (const auto& point : allPoints)
	{
		minPoint.x = std::min(minPoint.x, point.x);
		minPoint.y = std::min(minPoint.y, point.y);
		maxPoint.x = std::max(maxPoint.x, point.x);
		maxPoint.y = std::max(maxPoint.y, point.y);
	}

	fm::vec2 center = (maxPoint + minPoint) * 0.5f;
	fm::vec2 size = maxPoint - minPoint;

	// Return the approximate swept shape as a Transform2D
	// This approximation uses the mid-point of the original and new rotations for the swept shape's rotation
	float midRotation = (inOriginalTransform.rotation + inNewRotation) / 2.0f;
	return fm::Transform2D{center, size * 0.5f, midRotation};
}

AABB gComputeAABB(const fm::Transform2D& inTransform)
{
	float rotation = fm::to_radians(inTransform.rotation);
	float sin_r = std::abs(std::sinf(rotation));
	float cos_r = std::abs(std::cosf(rotation));

	// Compute the half-size of the rotated object
	float half_width = inTransform.halfSize.x * cos_r + inTransform.halfSize.y * sin_r;
	float half_height = inTransform.halfSize.x * sin_r + inTransform.halfSize.y * cos_r;
	fm::vec2 half_size = {half_width, half_height};

	AABB aabb;
	aabb.mMin = inTransform.position - half_size;
	aabb.mMax = inTransform.position + half_size;

	return aabb;
}
