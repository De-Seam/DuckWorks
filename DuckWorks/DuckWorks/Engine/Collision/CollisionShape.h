#pragma once
// Core includes
#include <Core/Math/Vector.h>

class Grid;

struct AABB
{
	Vec2 mMin = { FLT_MAX };
	Vec2 mMax = { FLT_MIN };
};

struct CollisionInfo
{
	bool mCollides;
	Vec2 mDirection;
	float mDepth;

	CollisionInfo(bool c, Vec2 dir, float d) : mCollides(c), mDirection(dir), mDepth(d) {}
	CollisionInfo() = default;
};

struct Rectangle
{
	Rectangle() = default;
	Rectangle(const Vec2& inPosition, const Vec2& inHalfSize, float inRotation) :
		mPosition(inPosition),
		mHalfSize(inHalfSize),
		mRotation(inRotation)
	{}

	Vec2 mPosition;
	Vec2 mHalfSize;
	float mRotation = 0.0f;

	static CollisionInfo sCollides(const Rectangle& inRectangleA, const Rectangle& inRectangleB);
	static Rectangle sComputeSweptShape(const Rectangle& inRectangleA, const Rectangle& inRectangleB);
	static AABB sComputeAABB(const Rectangle& inRectangle);
};

class CollisionShape
{
public:
	CollisionShape(const Rectangle& inRectangle, Grid& inGrid) :
		mRectangle(inRectangle),
		mGrid(&inGrid)
	{
		OnTransformChanged();
	}

	void SetPosition(const Vec2& inPosition) { Rectangle rectangle; rectangle.mPosition = inPosition; SetRectangle(rectangle); }
	void SetHalfSize(const Vec2& inHalfSize) { Rectangle rectangle; rectangle.mHalfSize = inHalfSize; SetRectangle(rectangle); }
	void SetRotation(float inRotation) { Rectangle rectangle; rectangle.mRotation = inRotation; SetRectangle(rectangle); }
	void SetRectangle(const Rectangle& inRectangle);

	const Vec2& GetPosition() const { return mRectangle.mPosition; }
	const Vec2& GetHalfSize() const { return mRectangle.mHalfSize; }
	float GetRotation() const { return mRectangle.mRotation; }
	const Rectangle& GetRectangle() const { return mRectangle; }
	const AABB& GetAABB() const { return mAABB; }

	void RemoveFromGrid();

	bool Collides(const CollisionShape& inOther) const;

private:
	void OnTransformChanged();
	void AddToGrid();
	void CalculateAABB();

	Rectangle mRectangle;
	AABB mAABB;

	Grid* mGrid = nullptr;

	IF_DEBUG( bool mIsInGrid = false;)
};