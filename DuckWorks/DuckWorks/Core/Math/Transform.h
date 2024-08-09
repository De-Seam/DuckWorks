#pragma once
#include <Core/Math/Quaternion.h>
#include <Core/Math/Vector.h>

struct Transform2D
{
	Vec2 mPosition;
	Vec2 mScale;
	float mRotation;

	Transform2D() :
		mPosition(0.f),
		mScale(1.0f),
		mRotation(0.0f) {}

	Transform2D(const Vec2& inPosition, const Vec2& inScale, float inRotation) :
		mPosition(inPosition),
		mScale(inScale),
		mRotation(inRotation) {}

	Transform2D(const Transform2D& i) = default;
	Transform2D& operator=(const Transform2D& inOther) = default;

	Transform2D operator*(const Transform2D& inOther) const
	{
		return
		{
			mPosition + inOther.mPosition,
			mScale * inOther.mScale,
			mRotation + inOther.mRotation
		};
	}

	Transform2D GetInverse() const
	{
		Vec2 inverse_position = -mPosition;
		Vec2 inverse_scale = Vec2(1.0f) / mScale;
		float inverse_rotation = -mRotation;
		return { inverse_position, inverse_scale, inverse_rotation };
	}

	//Transform2D operator+(const Transform2D& inOther) const
	//{
	//	return
	//	{
	//		mPosition + inOther.mPosition,
	//		mScale + inOther.mScale,
	//		mRotation + inOther.mRotation
	//	};
	//}
	//
	//Transform2D operator-(const Transform2D& inOther) const
	//{
	//	return
	//	{
	//		mPosition - inOther.mPosition,
	//		mScale - inOther.mScale,
	//		mRotation - inOther.mRotation
	//	};
	//}
};

struct Transform3D
{
	Vec3 mPosition;
	Vec3 mScale;
	Quat mRotation;
};
