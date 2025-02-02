#pragma once
#include <Core/Math/Vector.h>

struct Quat
{
	float mX, mY, mZ, mW;

	Quat() = default;

	Quat(float inX, float inY, float inZ, float inW)
		: mX(inX), mY(inY), mZ(inZ), mW(inW) {}

	Quat(float inX, float inY, float inZ)
	{
		float cy = std::cos(inZ * 0.5f);
		float sy = std::sin(inZ * 0.5f);
		float cp = std::cos(inY * 0.5f);
		float sp = std::sin(inY * 0.5f);
		float cr = std::cos(inX * 0.5f);
		float sr = std::sin(inX * 0.5f);

		mW = cr * cp * cy + sr * sp * sy;
		mX = sr * cp * cy - cr * sp * sy;
		mY = cr * sp * cy + sr * cp * sy;
		mZ = cr * cp * sy - sr * sp * cy;
	}

	Quat(const Vec3& inVector)
		: Quat(inVector.mX, inVector.mY, inVector.mZ) {}

	Quat(const Quat& inOther)
		: mX(inOther.mX), mY(inOther.mY), mZ(inOther.mZ), mW(inOther.mW) {}

	Quat& operator=(const Quat& inOther)
	{
		mX = inOther.mX;
		mY = inOther.mY;
		mZ = inOther.mZ;
		mW = inOther.mW;
		return *this;
	}

	Quat operator -() const { return {-mX, -mY, -mZ, -mW}; }
	Quat operator +(const Quat& inOther) const { return {mX + inOther.mX, mY + inOther.mY, mZ + inOther.mZ, mW + inOther.mW}; }
	Quat operator -(const Quat& inOther) const { return {mX - inOther.mX, mY - inOther.mY, mZ - inOther.mZ, mW - inOther.mW}; }
	Quat operator *(const float inMultiplier) const { return {mX * inMultiplier, mY * inMultiplier, mZ * inMultiplier, mW * inMultiplier}; }

	Quat operator *(const Quat& inOther) const
	{
		return
		{
			mW * inOther.mX + mX * inOther.mW + mY * inOther.mZ - mZ * inOther.mY,
			mW * inOther.mY - mX * inOther.mZ + mY * inOther.mW + mZ * inOther.mX,
			mW * inOther.mZ + mX * inOther.mY - mY * inOther.mX + mZ * inOther.mW,
			mW * inOther.mW - mX * inOther.mX - mY * inOther.mY - mZ * inOther.mZ
		};
	}

	const Quat& operator +=(const Quat& inOther) { return *this = *this + inOther; }
	const Quat& operator -=(const Quat& inOther) { return *this = *this - inOther; }
	const Quat& operator *=(const Quat& inOther) { return *this = *this * inOther; }
	const Quat& operator *=(const float inMultiplier) { return *this = *this * inMultiplier; }

	Vec3 Rotate(const Vec3& inVector) const;

	Vec3 GetEuler() const;
};

inline Quat gNormalize(const Quat& inQuat)
{
	float n = sqrt(inQuat.mX * inQuat.mX + inQuat.mY * inQuat.mY + inQuat.mZ * inQuat.mZ + inQuat.mW * inQuat.mW);
	if (n <= 0.f)
		return {0, 0, 0, 0};
	n = 1.f / n;
	return
	{
		inQuat.mX * n,
		inQuat.mY * n,
		inQuat.mZ * n,
		inQuat.mW * n
	};
}

inline Quat gAngleAxis(float inAngle, const Vec3& inVector)
{
	float a(inAngle);
	float s = std::sin(a * 0.5f);

	Vec3 xyz = inVector * s;
	return {xyz.mX, xyz.mY, xyz.mZ, std::cos(a * 0.5f)};
}
