#pragma once
// Core includes
#include <Core/Config.h>
#include <Core/Math/Vector.h>

// Std includes
#include <immintrin.h>

struct Quat;

struct Mat3
{
	Vec3 x, y, z;

	Mat3() {}

	Mat3(float i) :
		x(i, 0, 0),
		y(0, i, 0),
		z(0, 0, i) {}

	Mat3(Vec3 x, Vec3 y, Vec3 z)
		: x(x), y(y), z(z) {}

	Mat3(const Mat3& i)
		: x(i.x), y(i.y), z(i.z) {}

	Mat3& operator=(const Mat3& inOther)
	{
		x = inOther.x;
		y = inOther.y;
		z = inOther.z;
		return *this;
	}

	Vec3& operator[](size_t i)
	{
		assert(i < 3);
		return *(&x + i);
	}

	Mat3 operator*(Mat3& i)
	{
		return
		{
			{x[0] * i.x[0] + x[1] * i.y[0] + x[2] * i.z[0]},
			{y[0] * i.x[1] + y[1] * i.y[1] + y[2] * i.z[1]},
			{z[0] * i.x[2] + z[1] * i.y[2] + z[2] * i.z[2]}
		};
	}
};

struct Mat4
{
	Vec4 x, y, z, w;

	Mat4() {}

	Mat4(float i) :
		x(i, 0, 0, 0),
		y(0, i, 0, 0),
		z(0, 0, i, 0),
		w(0, 0, 0, i) {}

	Mat4(Vec4 x, Vec4 y, Vec4 z, Vec4 w)
		: x(x), y(y), z(z), w(w) {}

	Mat4(const Mat4& i)
		: x(i.x), y(i.y), z(i.z), w(i.w) {}

	Mat4& operator=(const Mat4& inOther)
	{
		x = inOther.x;
		y = inOther.y;
		z = inOther.z;
		w = inOther.w;
		return *this;
	}

	Mat4(const Mat3& i)
		: x(i.x), y(i.y), z(i.z), w(0.f, 0.f, 0.f, 1.f) {}

	Vec4& operator[](size_t i)
	{
		assert(i < 4);
		return *(&x + i);
	}

	const Vec4& operator[](size_t i) const
	{
		assert(i < 4);
		return *(&x + i);
	}

	Mat4 operator*(const Mat4& i) const
	{
		Mat4 result;
#ifdef ENABLE_SIMD
		__m128 row1 = _mm_loadu_ps(&x.mX);
		__m128 row2 = _mm_loadu_ps(&y.mX);
		__m128 row3 = _mm_loadu_ps(&z.mX);
		__m128 row4 = _mm_loadu_ps(&w.mX);
		
		for (int j = 0; j < 4; ++j) 
		{
		    __m128 col = _mm_set_ps(i.w[j], i.z[j], i.y[j], i.x[j]);
		
		    __m128 r1 = _mm_mul_ps(row1, col);
		    __m128 r2 = _mm_mul_ps(row2, col);
		    __m128 r3 = _mm_mul_ps(row3, col);
		    __m128 r4 = _mm_mul_ps(row4, col);
		
		    r1 = _mm_hadd_ps(r1, r1);
		    r2 = _mm_hadd_ps(r2, r2);
		    r3 = _mm_hadd_ps(r3, r3);
		    r4 = _mm_hadd_ps(r4, r4);
		
		    float res1[4], res2[4], res3[4], res4[4];
		    _mm_storeu_ps(res1, r1);
		    _mm_storeu_ps(res2, r2);
		    _mm_storeu_ps(res3, r3);
		    _mm_storeu_ps(res4, r4);
		
		    result.x[j] = res1[0] + res1[1];
		    result.y[j] = res2[0] + res2[1];
		    result.z[j] = res3[0] + res3[1];
		    result.w[j] = res4[0] + res4[1];
		}
#else
		result.x.mX = x.mX * i.x.mX + x.mY * i.y.mX + x.mZ * i.z.mX + x.mW * i.w.mX;
		result.x.mY = x.mX * i.x.mY + x.mY * i.y.mY + x.mZ * i.z.mY + x.mW * i.w.mY;
		result.x.mZ = x.mX * i.x.mZ + x.mY * i.y.mZ + x.mZ * i.z.mZ + x.mW * i.w.mZ;
		result.x.mW = x.mX * i.x.mW + x.mY * i.y.mW + x.mZ * i.z.mW + x.mW * i.w.mW;

		result.y.mX = y.mX * i.x.mX + y.mY * i.y.mX + y.mZ * i.z.mX + y.mW * i.w.mX;
		result.y.mY = y.mX * i.x.mY + y.mY * i.y.mY + y.mZ * i.z.mY + y.mW * i.w.mY;
		result.y.mZ = y.mX * i.x.mZ + y.mY * i.y.mZ + y.mZ * i.z.mZ + y.mW * i.w.mZ;
		result.y.mW = y.mX * i.x.mW + y.mY * i.y.mW + y.mZ * i.z.mW + y.mW * i.w.mW;

		result.z.mX = z.mX * i.x.mX + z.mY * i.y.mX + z.mZ * i.z.mX + z.mW * i.w.mX;
		result.z.mY = z.mX * i.x.mY + z.mY * i.y.mY + z.mZ * i.z.mY + z.mW * i.w.mY;
		result.z.mZ = z.mX * i.x.mZ + z.mY * i.y.mZ + z.mZ * i.z.mZ + z.mW * i.w.mZ;
		result.z.mW = z.mX * i.x.mW + z.mY * i.y.mW + z.mZ * i.z.mW + z.mW * i.w.mW;

		result.w.mX = w.mX * i.x.mX + w.mY * i.y.mX + w.mZ * i.z.mX + w.mW * i.w.mX;
		result.w.mY = w.mX * i.x.mY + w.mY * i.y.mY + w.mZ * i.z.mY + w.mW * i.w.mY;
		result.w.mZ = w.mX * i.x.mZ + w.mY * i.y.mZ + w.mZ * i.z.mZ + w.mW * i.w.mZ;
		result.w.mW = w.mX * i.x.mW + w.mY * i.y.mW + w.mZ * i.z.mW + w.mW * i.w.mW;
#endif
		return result;
	}

	const Mat4& operator *=(const Mat4& i) { return *this = *this * i; }

	std::string to_string()
	{
		return std::to_string(x.mX) + " : " + std::to_string(x.mY) + " : " + std::to_string(x.mZ) + " : " +
			std::to_string(x.mW) + "\n" +
			std::to_string(y.mX) + " : " + std::to_string(y.mY) + " : " + std::to_string(y.mZ) + " : " +
			std::to_string(y.mW) + "\n" +
			std::to_string(z.mX) + " : " + std::to_string(z.mY) + " : " + std::to_string(z.mZ) + " : " +
			std::to_string(z.mW) + "\n" +
			std::to_string(w.mX) + " : " + std::to_string(w.mY) + " : " + std::to_string(w.mZ) + " : " +
			std::to_string(w.mW) + "\n";
	}
};

Mat4 gTranslate(Mat4 matrix, Vec3 vector);

Mat4 gScale(Mat4 matrix, Vec3 vector);

Mat4 gRotate(Mat4 m, float angle, Vec3 v);

Mat4 GetPerspective(float fovy, float aspect, float znear, float zfar);

Mat4 gLookAt(const Vec3& eye, const Vec3& target, const Vec3& up);

Mat3 gToMat3(const Quat& q);

Mat4 gToMat4(const Quat& q);
