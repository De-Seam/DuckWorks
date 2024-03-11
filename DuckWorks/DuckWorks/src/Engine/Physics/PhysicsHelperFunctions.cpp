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
std::pair<float, float> gProjectRectangle(const fm::Transform2D& rect, const fm::vec2& axis) {
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
    for (int i = 1; i < 4; ++i) {
        float projection = axis.dot(corners[i]);
        if (projection < min) min = projection;
        if (projection > max) max = projection;
    }
    return { min, max };
}

// Check for overlap on a single axis
bool gOverlaps(const std::pair<float, float>& a, const std::pair<float, float>& b) {
    return a.first <= b.second && b.first <= a.second;
}

// Main collision detection function using SAT
bool gCollides(const fm::Transform2D& inTransformA, const fm::Transform2D& inTransformB) {
    float radA = fm::to_radians(inTransformA.rotation);
    float radB = fm::to_radians(inTransformB.rotation);

    // Axes to test against, derived from the edges of both rectangles
    std::vector<fm::vec2> axes = {
        {std::cos(radA), std::sin(radA)}, // A's x-axis
        {std::cos(radA + fm::pi() / 2), std::sin(radA + fm::pi() / 2)}, // A's y-axis
        {std::cos(radB), std::sin(radB)}, // B's x-axis
        {std::cos(radB + fm::pi() / 2), std::sin(radB + fm::pi() / 2)} // B's y-axis
    };

    for (const auto& axis : axes) {
        auto projA = gProjectRectangle(inTransformA, axis);
        auto projB = gProjectRectangle(inTransformB, axis);
        if (!gOverlaps(projA, projB)) {
            return false; // No collision if there's no overlap on this axis
        }
    }

    return true; // Collision detected
}

AABB gComputeAABB(const fm::Transform2D& inTransform)
{
    float rotation = fm::to_radians(inTransform.rotation);
    float sin_r = std::abs(std::sinf(rotation));
    float cos_r = std::abs(std::cosf(rotation));

    // Compute the half-size of the rotated object
    float half_width = inTransform.halfSize.x * cos_r + inTransform.halfSize.y * sin_r;
    float half_height = inTransform.halfSize.x * sin_r + inTransform.halfSize.y * cos_r;
    fm::vec2 half_size = { half_width, half_height };

    AABB aabb;
    aabb.mMin = inTransform.position - half_size;
    aabb.mMax = inTransform.position + half_size;

    return aabb;
}

