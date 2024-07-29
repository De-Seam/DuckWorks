#pragma once
// Core includes
#include <Core/Utilities/Types.h>
#include <Core/Utilities/Utilities.h>

#include <cassert>
#include <cmath>

#include <External/SFML/System/Vector2.hpp>

#pragma warning (push)
#pragma warning (disable : 4201) //to avoid nameless struct / union warning.

struct IVec2
{
	int32 mX, mY;

	IVec2(int32 inX, int32 inY)
		: mX(inX), mY(inY) {}

	IVec2(const int32 inInteger)
		: IVec2(inInteger, inInteger) {}

	IVec2()
		: IVec2(0) {}

	IVec2(const IVec2& inOther)
		: IVec2(inOther.mX, inOther.mY) {}

	IVec2(const sf::Vector2i inVector)
		: IVec2(inVector.x, inVector.y) {}

	IVec2(const sf::Vector2u inVector)
		: IVec2(static_cast<int>(inVector.x), static_cast<int>(inVector.y)) {}

	IVec2& operator=(const IVec2& inOther)
	{
		mX = inOther.mX;
		mY = inOther.mY;
		return *this;
	}

	int32& operator[](size_t inIndex)
	{
		assert(inIndex < 2);
		return *(&mX + inIndex);
	}

	IVec2 operator -() const { return {-mX, -mY}; }
	IVec2 operator +(const IVec2& inOther) const { return {mX + inOther.mX, mY + inOther.mY}; }
	IVec2 operator -(const IVec2& inOther) const { return {mX - inOther.mX, mY - inOther.mY}; }
	IVec2 operator *(const IVec2& inOther) const { return {mX * inOther.mX, mY * inOther.mY}; }
	IVec2 operator *(const int32 inMultiplier) const { return {mX * inMultiplier, mY * inMultiplier}; }
	IVec2 operator /(const IVec2& inOther) const { return {mX / inOther.mX, mY / inOther.mY}; }
	IVec2 operator /(const int32 inDivider) const { return {mX / inDivider, mY / inDivider}; }
	const IVec2& operator +=(const IVec2& inOther) { return *this = *this + inOther; }
	const IVec2& operator -=(const IVec2& inOther) { return *this = *this - inOther; }
	const IVec2& operator *=(const IVec2& inOther) { return *this = *this * inOther; }
	const IVec2& operator *=(const int32 inMultiplier) { return *this = *this * inMultiplier; }
	const IVec2& operator /=(const IVec2& inOther) { return *this = *this / inOther; }
	const IVec2& operator /=(const int32 inDivider) { return *this = *this / inDivider; }
};

void to_json(Json& outJson, const IVec2& inVariable);
void from_json(const Json& inJson, IVec2& outVariable);

struct Vec2
{
	float mX, mY;

	Vec2(float x, float y)
		: mX(x), mY(y) {}

	Vec2(const float i)
		: Vec2(i, i) {}

	Vec2()
		: Vec2(0) {}

	Vec2(const Vec2& i)
		: Vec2(i.mX, i.mY) {}

	Vec2(const sf::Vector2f i)
		: Vec2(i.x, i.y) {}

	Vec2& operator=(const Vec2& inOther)
	{
		mX = inOther.mX;
		mY = inOther.mY;
		return *this;
	}

	Vec2(const IVec2& i)
		: mX(static_cast<float>(i.mX)), mY(static_cast<float>(i.mY)) {}

	Vec2& operator=(const IVec2& inOther)
	{
		mX = static_cast<float>(inOther.mX);
		mY = static_cast<float>(inOther.mY);
		return *this;
	}

	float& operator[](size_t i)
	{
		assert(i < 2);
		return *(&mX + i);
	}

	const float& operator[](size_t i) const
	{
		assert(i < 2);
		return *(&mX + i);
	}

	IVec2 ToIVec2() const
	{
		return {static_cast<int32>(mX), static_cast<int32>(mY)};
	}

	Vec2 operator -() const { return {-mX, -mY}; }
	Vec2 operator +(const Vec2& inOther) const { return {mX + inOther.mX, mY + inOther.mY}; }
	Vec2 operator -(const Vec2& inOther) const { return {mX - inOther.mX, mY - inOther.mY}; }
	Vec2 operator *(const Vec2& inOther) const { return {mX * inOther.mX, mY * inOther.mY}; }
	Vec2 operator *(const float inMultiplier) const { return {mX * inMultiplier, mY * inMultiplier}; }
	Vec2 operator /(const Vec2& inOther) const { return {mX / inOther.mX, mY / inOther.mY}; }
	Vec2 operator /(const float inDivider) const { return {mX / inDivider, mY / inDivider}; }
	const Vec2& operator +=(const Vec2& inOther) { return *this = *this + inOther; }
	const Vec2& operator -=(const Vec2& inOther) { return *this = *this - inOther; }
	const Vec2& operator *=(const Vec2& inOther) { return *this = *this * inOther; }
	const Vec2& operator *=(const float inMultiplier) { return *this = *this * inMultiplier; }
	const Vec2& operator /=(const Vec2& inOther) { return *this = *this / inOther; }
	const Vec2& operator /=(const float inDivider) { return *this = *this / inDivider; }

	bool operator==(const Vec2& inOther) const { return mX == inOther.mX && mY == inOther.mY; }
	bool operator!=(const Vec2& inOther) const { return !(*this == inOther); }

	[[nodiscard]] float Dot(const Vec2& inOther) const
	{
		return (mX * inOther.mX + mY * inOther.mY);
	}

	[[nodiscard]] Vec2 Cross(const Vec2& inOther) const
	{
		return Vec2(
			mY * inOther.mX - mX * inOther.mY,
			mX * inOther.mY - mY * inOther.mX);
	}

	[[nodiscard]] float Length2() const
	{
		return mX * mX + mY * mY;
	}

	[[nodiscard]] float Length() const
	{
		return std::sqrt(Length2());
	}

	[[nodiscard]] Vec2 Normalize() const
	{
		float l = Length();
		return {mX / l, mY / l};
	}
};

void to_json(Json& outJson, const Vec2& inVariable);
void from_json(const Json& inJson, Vec2& outVariable);

struct Vec3
{
	float mX, mY, mZ;

	Vec3(float x, float y, float z)
		: mX(x), mY(y), mZ(z) {}

	Vec3(const float i)
		: Vec3(i, i, i) {}

	Vec3()
		: Vec3(0) {}

	Vec3(const Vec3& i)
		: Vec3(i.mX, i.mY, i.mZ) {}

	Vec3& operator=(const Vec3& inOther)
	{
		mX = inOther.mX;
		mY = inOther.mY;
		mZ = inOther.mZ;
		return *this;
	}

	float& operator[](size_t i)
	{
		assert(i < 3);
		return *(&mX + i);
	}

	Vec3 operator -() const { return {-mX, -mY, -mZ}; }
	Vec3 operator +(const Vec3& i) const { return {mX + i.mX, mY + i.mY, mZ + i.mZ}; }
	Vec3 operator -(const Vec3& i) const { return {mX - i.mX, mY - i.mY, mZ - i.mZ}; }
	Vec3 operator *(const Vec3& i) const { return {mX * i.mX, mY * i.mY, mZ * i.mZ}; }
	Vec3 operator *(const float i) const { return {mX * i, mY * i, mZ * i}; }
	Vec3 operator /(const Vec3& i) const { return {mX / i.mX, mY / i.mY, mZ / i.mZ}; }
	Vec3 operator /(const float i) const { return {mX / i, mY / i, mZ / i}; }
	const Vec3& operator +=(const Vec3& i) { return *this = *this + i; }
	const Vec3& operator -=(const Vec3& i) { return *this = *this - i; }
	const Vec3& operator *=(const Vec3& i) { return *this = *this * i; }
	const Vec3& operator *=(const float i) { return *this = *this * i; }
	const Vec3& operator /=(const Vec3& i) { return *this = *this / i; }
	const Vec3& operator /=(const float i) { return *this = *this / i; }

	bool operator==(const Vec3& i) { return mX == i.mX && mY == i.mY && mZ == i.mZ; }

	[[nodiscard]] uint32 GetARGB() const
	{
		const float t_r = gClamp<float>(mX, 0, 1);
		const float t_g = gClamp<float>(mY, 0, 1);
		const float t_b = gClamp<float>(mZ, 0, 1);
		return (uint32)255 | (static_cast<uint8_t>(t_r * 255) << 16) | (static_cast<uint8_t>(t_g * 255) << 8) | (static_cast<
			uint8_t>(t_b * 255));
	}

	[[nodiscard]] uint32_t GetRGBA() const
	{
		const float t_r = gClamp<float>(mX, 0, 1);
		const float t_g = gClamp<float>(mY, 0, 1);
		const float t_b = gClamp<float>(mZ, 0, 1);
		return static_cast<uint32_t>((static_cast<uint8_t>(t_r * 255) << 24) | (static_cast<uint8_t>(t_g * 255) << 16) |
			(static_cast<uint8_t>(t_b * 255) << 8) | 255);
	}

	[[nodiscard]] float Dot(const Vec3& i) const
	{
		return (mX * i.mX + mY * i.mY + mZ * i.mZ);
	}

	[[nodiscard]] Vec3 Cross(const Vec3& i) const
	{
		return Vec3(
			mY * i.mZ - mZ * i.mY,
			mZ * i.mX - mX * i.mZ,
			mX * i.mY - mY * i.mX);
	}
};

struct alignas(4 * sizeof(float)) Vec4
{
	float mX, mY, mZ, mW;

	Vec4(float x, float y, float z, float w)
		: mX(x), mY(y), mZ(z), mW(w) {}

	Vec4(const float i)
		: Vec4(i, i, i, i) {}

	Vec4()
		: Vec4(0) {}

	Vec4(const Vec4& i)
		: Vec4(i.mX, i.mY, i.mZ, i.mW) {}

	Vec4& operator=(const Vec4& inOther)
	{
		mX = inOther.mX;
		mY = inOther.mY;
		mZ = inOther.mZ;
		mW = inOther.mW;
		return *this;
	}

	Vec4(const Vec3& i)
		: Vec4(i.mX, i.mY, i.mZ, 0.f) {}

	float& operator[](size_t i)
	{
		assert(i < 4);
		return *(&mX + i);
	}

	const float& operator[](size_t i) const
	{
		assert(i < 4);
		return *(&mX + i);
	}

	Vec4 operator -() const { return {-mX, -mY, -mZ, -mW}; }
	Vec4 operator +(const Vec4& i) const { return {mX + i.mX, mY + i.mY, mZ + i.mZ, mW + i.mW}; }
	Vec4 operator -(const Vec4& i) const { return {mX - i.mX, mY - i.mY, mZ - i.mZ, mW - i.mW}; }
	Vec4 operator *(const Vec4& i) const { return {mX * i.mX, mY * i.mY, mZ * i.mZ, mW * i.mW}; }
	Vec4 operator *(const float i) const { return {mX * i, mY * i, mZ * i, mW * i}; }
	Vec4 operator /(const Vec4& i) const { return {mX / i.mX, mY / i.mY, mZ / i.mZ, mW / i.mW}; }
	Vec4 operator /(const float i) const { return {mX / i, mY / i, mZ / i, mW / i}; }
	const Vec4& operator +=(const Vec4& i) { return *this = *this + i; }
	const Vec4& operator -=(const Vec4& i) { return *this = *this - i; }
	const Vec4& operator *=(const Vec4& i) { return *this = *this * i; }
	const Vec4& operator *=(const float i) { return *this = *this * i; }
	const Vec4& operator /=(const Vec4& i) { return *this = *this / i; }
	const Vec4& operator /=(const float i) { return *this = *this / i; }

	[[nodiscard]] uint32_t GetARGB() const
	{
		const float t_a = gClamp<float>(mW, 0, 1);
		const float t_r = gClamp<float>(mX, 0, 1);
		const float t_g = gClamp<float>(mY, 0, 1);
		const float t_b = gClamp<float>(mZ, 0, 1);
		return static_cast<uint32_t>((static_cast<uint8_t>(t_a * 255) << 24) | (static_cast<uint8_t>(t_r * 255) << 16) |
			(static_cast<uint8_t>(t_g * 255) << 8) | (static_cast<uint8_t>(t_b * 255)));
	}

	[[nodiscard]] uint32_t GetRGBA() const
	{
		const float t_a = gClamp<float>(mW, 0, 1);
		const float t_r = gClamp<float>(mX, 0, 1);
		const float t_g = gClamp<float>(mY, 0, 1);
		const float t_b = gClamp<float>(mZ, 0, 1);
		return static_cast<uint32_t>((static_cast<uint8_t>(t_r * 255) << 24) | (static_cast<uint8_t>(t_g * 255) << 16) |
			(static_cast<uint8_t>(t_b * 255) << 8) | (static_cast<uint8_t>(t_a * 255)));
	}
};

struct alignas(4 * sizeof(float)) IVec4
{
	int32 mX, mY, mZ, mW;

	IVec4(int32 x, int32 y, int32 z, int32 w)
		: mX(x), mY(y), mZ(z), mW(w) {}

	IVec4(const int32 i)
		: IVec4(i, i, i, i) {}

	IVec4()
		: IVec4(0) {}

	IVec4(const IVec4& i)
		: IVec4(i.mX, i.mY, i.mZ, i.mW) {}

	IVec4& operator=(const IVec4& inOther)
	{
		mX = inOther.mX;
		mY = inOther.mY;
		mZ = inOther.mZ;
		mW = inOther.mW;
		return *this;
	}

	IVec4(const IVec2& xy, const IVec2& zw)
		: IVec4(xy.mX, xy.mY, zw.mX, zw.mY) {}

	int32& operator[](size_t i)
	{
		assert(i < 4);
		return *(&mX + i);
	}

	const int32& operator[](size_t i) const
	{
		assert(i < 4);
		return *(&mX + i);
	}

	IVec4 operator -() const { return {-mX, -mY, -mZ, -mW}; }
	IVec4 operator +(const IVec4& i) const { return {mX + i.mX, mY + i.mY, mZ + i.mZ, mW + i.mW}; }
	IVec4 operator -(const IVec4& i) const { return {mX - i.mX, mY - i.mY, mZ - i.mZ, mW - i.mW}; }
	IVec4 operator *(const IVec4& i) const { return {mX * i.mX, mY * i.mY, mZ * i.mZ, mW * i.mW}; }
	IVec4 operator *(const int32 i) const { return {mX * i, mY * i, mZ * i, mW * i}; }
	IVec4 operator /(const IVec4& i) const { return {mX / i.mX, mY / i.mY, mZ / i.mZ, mW / i.mW}; }
	IVec4 operator /(const int32 i) const { return {mX / i, mY / i, mZ / i, mW / i}; }
	const IVec4& operator +=(const IVec4& i) { return *this = *this + i; }
	const IVec4& operator -=(const IVec4& i) { return *this = *this - i; }
	const IVec4& operator *=(const IVec4& i) { return *this = *this * i; }
	const IVec4& operator *=(const int32 i) { return *this = *this * i; }
	const IVec4& operator /=(const IVec4& i) { return *this = *this / i; }
	const IVec4& operator /=(const int32 i) { return *this = *this / i; }
};

inline Vec2 gNormalize(const Vec2& i)
{
	float mag = std::sqrt(i.mX * i.mX + i.mY * i.mY);
	assert(mag != 0.f && "Try using normalize_safe instead");
	float inv = 1.f / mag;
	return {i.mX * inv, i.mY * inv};
}

inline Vec2 gNormalizeSafe(const Vec2& i)
{
	float mag = std::sqrt(i.mX * i.mX + i.mY * i.mY);
	if (mag == 0.f)
		mag = 1.f;
	float inv = 1.f / mag;
	return {i.mX * inv, i.mY * inv};
}

inline Vec3 gNormalize(const Vec3& i)
{
	float mag = std::sqrt(i.mX * i.mX + i.mY * i.mY + i.mZ * i.mZ);
	assert(mag != 0.f && "Try using normalize_safe instead");
	float inv = 1.f / mag;
	return {i.mX * inv, i.mY * inv, i.mZ * inv};
}

inline Vec3 gNormalizeSafe(const Vec3& i)
{
	float mag = std::sqrt(i.mX * i.mX + i.mY * i.mY + i.mZ * i.mZ);
	if (mag == 0.f)
		mag = 1.f;
	float inv = 1.f / mag;
	return {i.mX * inv, i.mY * inv, i.mZ * inv};
}

inline Vec4 gNormalize(const Vec4& i)
{
	float mag = std::sqrt(i.mX * i.mX + i.mY * i.mY + i.mZ * i.mZ);
	assert(mag != 0.f && "Try using normalize_safe instead");
	float inv = 1.f / mag;
	return {i.mX * inv, i.mY * inv, i.mZ * inv, i.mW * inv};
}

inline Vec4 gNormalizeSafe(const Vec4& i)
{
	float mag = std::sqrt(i.mX * i.mX + i.mY * i.mY + i.mZ * i.mZ);
	if (mag == 0.f)
		mag = 1.f;
	float inv = 1.f / mag;
	return {i.mX * inv, i.mY * inv, i.mZ * inv, i.mW * inv};
}

inline Vec2 gClamp2(Vec2 x, Vec2 a, Vec2 b)
{
	return
	{
		gClamp(x.mX, a.mX, b.mX),
		gClamp(x.mY, a.mY, b.mY)
	};
}

inline IVec2 gClamp2(IVec2 x, IVec2 a, IVec2 b)
{
	return
	{
		gClamp(x.mX, a.mX, b.mX),
		gClamp(x.mY, a.mY, b.mY)
	};
}

inline Vec2 gMax2(Vec2 a, Vec2 b)
{
	return
	{
		gMax(a.mX, b.mX),
		gMax(a.mY, b.mY)
	};
}

inline Vec2 gMin2(Vec2 a, Vec2 b)
{
	return
	{
		gMin(a.mX, b.mX),
		gMin(a.mY, b.mY)
	};
}

inline Vec2 gLerp2(Vec2 a, Vec2 b, float t)
{
	return a + (b - a) * t;
}

inline Vec3 gLerp3(Vec3 a, Vec3 b, float t)
{
	return a + (b - a) * t;
}

#pragma warning (pop)
