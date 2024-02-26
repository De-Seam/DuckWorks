#pragma once
#include "Core/Math/FMath.h"

#define MIN_ZOOM 0.05f

class Camera
{
public:
	Camera(fm::vec2 inPosition = {0, 0}, fm::vec2 inSize = {1920.f, 1080.f}, float inZoom = 1.f);

	virtual void Update(float inDeltaTime);
	void SetPosition(fm::vec2 inPosition);
	void SetSize(fm::vec2 inSize);
	void SetZoom(float inZoom);
	void SnapZoom(float inZoom);
	void SetZoomSpeed(float inZoomSpeed);

	fm::vec2 GetPosition() const { return mPosition; }
	fm::vec2 GetScale() const { return mScale; }
	float GetZoom() const { return mZoom; }

private:
	fm::vec2 mPosition = {0.f, 0.f};
	fm::vec2 mSize = {1920.f, 1080.f};
	fm::vec2 mSizeInverse = {1.f / 1920.f, 1.f / 1080.f};
	fm::vec2 mScale = {1.f, 1.f};
	float mZoomSpeed = 40;
	float mTargetZoom = 1.f;
	float mZoom = 1;
};
