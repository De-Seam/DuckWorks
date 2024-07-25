#pragma once
#include <Core/Math/Quaternion.h>
#include <Core/Math/Vector.h>

struct Transform2D
{
	Vec2 mPosition;
	Vec2 mHalfSize;
	float mRotation;

	Transform2D() :
		mPosition(0.f),
		mHalfSize(16.f),
		mRotation(0.f) {}

	Transform2D(const Vec2& inPosition, const Vec2& inSize, float inRotation) :
		mPosition(inPosition),
		mHalfSize(inSize),
		mRotation(inRotation) {}

	Transform2D(const Transform2D& i) = default;
	Transform2D& operator=(const Transform2D& inOther) = default;

	Transform2D operator+(const Transform2D& inOther) const
	{
		return
		{
			mPosition + inOther.mPosition,
			mHalfSize + inOther.mHalfSize,
			mRotation + inOther.mRotation
		};
	}

	Transform2D operator-(const Transform2D& inOther) const
	{
		return
		{
			mPosition - inOther.mPosition,
			mHalfSize - inOther.mHalfSize,
			mRotation - inOther.mRotation
		};
	}
};

struct Transform3D
{
	Vec3 mPosition;
	Vec3 mScale;
	Quat mRotation;
};
