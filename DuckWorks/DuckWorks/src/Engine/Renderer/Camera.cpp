#include "Precomp.h"
#include "Engine/Renderer/Camera.h"

#include "Engine/Renderer/Renderer.h"

Camera::Camera(fm::vec2 inPosition, fm::vec2 inSize, float inZoom) :
	mPosition(inPosition),
	mSize(inSize),
	mZoom(inZoom)
{
	mSizeInverse = fm::vec2{1.f} / mSize;
	mScale = fm::vec2{1.f, 1.f} * mZoom;
}

void Camera::Update(float inDeltaTime)
{
	fm::vec2 windowSize = gRenderer.GetWindowSize();

	mScale = windowSize / mSize * mZoom;

	mZoom = fm::lerp(mZoom, mTargetZoom, inDeltaTime * mZoomSpeed);
}

void Camera::SetPosition(fm::vec2 inPosition)
{
	mPosition = inPosition;
}

void Camera::SetSize(fm::vec2 inSize)
{
	mSize = inSize;
	mSizeInverse = fm::vec2{1.f} / mSize;
}

void Camera::SetZoom(float inZoom)
{
	mTargetZoom = fm::max(inZoom, MIN_ZOOM);
}

void Camera::SnapZoom(float inZoom)
{
	mTargetZoom = fm::max(inZoom, MIN_ZOOM);
	mZoom = mTargetZoom;
}

void Camera::SetZoomSpeed(float inZoomSpeed)
{
	mZoomSpeed = inZoomSpeed;
}
