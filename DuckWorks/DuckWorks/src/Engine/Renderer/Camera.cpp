#include "Precomp.h"
#include "Engine/Renderer/Camera.h"

#include "Engine/Renderer/Renderer.h"

Camera::Camera(fm::vec2 inPosition, fm::vec2 inSize, float inZoom) :
	mPosition(inPosition),
	mSize(inSize),
	mZoom(inZoom)
{
	mSizeInverse = fm::vec2{1.f} / mSize;

	fm::ivec2 size = {inSize.to_ivec2()};
	mRenderTexture = SDL_CreateTexture(gRenderer.GetRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size.x, size.y);
	if (!mRenderTexture)
	{
		gLog(LogType::Error, "Error creating render target texture: %s\n", SDL_GetError());
	}
}

void Camera::Update(float inDeltaTime)
{
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

	SDL_DestroyTexture(mRenderTexture);
	const fm::ivec2& window_size = gRenderer.GetWindowSize();
	mRenderTexture = SDL_CreateTexture(gRenderer.GetRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, window_size.x, window_size.y);
}

void Camera::SetZoom(float inZoom)
{
	mTargetZoom = fm::max(inZoom, MIN_ZOOM);
}

void Camera::SetZoomSpeed(float inZoomSpeed)
{
	mZoomSpeed = inZoomSpeed;
}

void Camera::SnapZoom(float inZoom)
{
	mTargetZoom = fm::max(inZoom, MIN_ZOOM);
	mZoom = mTargetZoom;
}

void Camera::SetRenderTexture(SDL_Texture* inRenderTexture)
{
	mRenderTexture = inRenderTexture;
}
