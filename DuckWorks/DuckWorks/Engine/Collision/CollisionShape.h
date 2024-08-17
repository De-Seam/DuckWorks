#pragma once
// Core includes
#include <Core/Math/Vector.h>

class Grid;

struct AABB
{
	Vec2 mMin = { FLT_MAX };
	Vec2 mMax = { FLT_MIN };
};

class CollisionShape
{
public:
	CollisionShape(const Vec2& inPosition, const Vec2& inHalfSize, float inRotation, Grid& inGrid) :
		mPosition(inPosition),
		mHalfSize(inHalfSize),
		mRotation(inRotation),
		mGrid(&inGrid)
	{
		OnTransformChanged();
	}

	void SetPosition(const Vec2& inPosition) { mPosition = inPosition; OnTransformChanged(); }
	void SetHalfSize(const Vec2& inHalfSize) { mHalfSize = inHalfSize; OnTransformChanged(); }
	void SetRotation(float inRotation) { mRotation = inRotation; OnTransformChanged(); }

	const Vec2& GetPosition() const { return mPosition; }
	const Vec2& GetHalfSize() const { return mHalfSize; }
	float GetRotation() const { return mRotation; }
	const AABB& GetAABB() const { return mAABB; }

	void RemoveFromGrid();

private:
	void OnTransformChanged();
	void AddToGrid();

	Vec2 mPosition;
	Vec2 mHalfSize;
	float mRotation = 0.0f;
	AABB mAABB;

	Grid* mGrid = nullptr;

	IF_DEBUG( bool mIsInGrid = false;)
};