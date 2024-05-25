#include "Precomp.h"
#include "Engine/Renderer/Camera.h"

#include "Engine/Renderer/Renderer.h"

RTTI_CLASS_DEFINITION(Camera)

Json Camera::Serialize()
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mPosition);
	JSON_SAVE(json, mSize);
	JSON_SAVE(json, mSizeInverse);
	JSON_SAVE(json, mZoomSpeed);
	JSON_SAVE(json, mTargetZoom);
	JSON_SAVE(json, mZoom);

	return json;
}

void Camera::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	JSON_TRY_LOAD(inJson, mPosition);
	JSON_TRY_LOAD(inJson, mSize);
	JSON_TRY_LOAD(inJson, mSizeInverse);
	JSON_TRY_LOAD(inJson, mZoomSpeed);
	JSON_TRY_LOAD(inJson, mTargetZoom);
	JSON_TRY_LOAD(inJson, mZoom);
}

Camera::Camera(const ConstructParameters& inParameters)
	: Base(inParameters),
	mPosition(inParameters.mPosition),
	mSize(inParameters.mSize),
	mZoom(inParameters.mZoom)
{
	mSizeInverse = fm::vec2{1.f} / mSize;

	fm::ivec2 size = {mSize.to_ivec2()};
	mRenderTexture = SDL_CreateTexture(gRenderer.GetRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size.x, size.y);
	if (!mRenderTexture)
	{
		gLog(ELogType::Error, "Error creating render target texture: %s\n", SDL_GetError());
	}
}

void Camera::Update(float inDeltaTime)
{
	mPosition = lerp2(mPosition, mTargetPosition, inDeltaTime * mPositionSpeed);
	mZoom = fm::lerp(mZoom, mTargetZoom, inDeltaTime * mZoomSpeed);
}

void Camera::SetPosition(fm::vec2 inPosition)
{
	mTargetPosition = inPosition;
}

void Camera::SnapPosition(fm::vec2 inPosition)
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
