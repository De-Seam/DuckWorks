#pragma once

constexpr float gPi()
{
	return 3.14159265358979323846f;
}

inline float gToRadians(float inDegrees)
{
	return inDegrees * 0.01745329251994329576923690768489f;
}

inline float gToDegrees(float inRadians)
{
	return inRadians * 57.295779513082320876798154814105f;
}

inline float gLerp(float a, float b, float t)
{
	return a + (b - a) * t;
}