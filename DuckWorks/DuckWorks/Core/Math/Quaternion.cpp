#include <Precomp.h>
#include <Core/Math/HelperFunctions.h>
#include <Core/Math/Quaternion.h>

Vec3 Quat::Rotate(const Vec3& v) const
{
	Quat p(v.mX, v.mY, v.mZ, 0);
	Quat q_inv(-mX, -mY, -mZ, mW);
	Quat result = (*this) * p * q_inv;

	return Vec3(result.mX, result.mY, result.mZ);
}

Vec3 Quat::GetEuler() const
{
	Vec3 angles;

	// yaw (z-axis rotation)
	{
		float siny_cosp = 2.f * (mY * mZ + mW * mX);
		float cosy_cosp = mW * mW - mX * mX - mY * mY + mZ * mZ;

		angles.mX = std::atan2(siny_cosp, cosy_cosp);
	}

	// pitch (y-axis rotation)
	{
		float sinp = 2.f * (mW * mY - mZ * mX);
		if (std::abs(sinp) >= 1.f)
			angles.mY = std::copysign(gPi() / 2.f, sinp); // use 90 degrees if out of range
		else
			angles.mY = std::asin(sinp);
	}

	// roll (x-axis rotation)
	{
		float t_y = 2.f * (mX * mY + mW * mZ);
		float t_x = mW * mW + mX * mX - mY * mY - mZ * mZ;

		angles.mZ = std::atan2(t_y, t_x);
	}

	return angles;
}
