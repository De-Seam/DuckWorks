#include "Precomp.h"
#include "Engine/Renderer/Camera.h"

#include "Engine/Renderer/Renderer.h"

RTTI_CLASS_DEFINITION(Camera, StandardAllocator)

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

	JSON_LOAD(inJson, mPosition);
	JSON_LOAD(inJson, mSize);
	JSON_LOAD(inJson, mSizeInverse);
	JSON_LOAD(inJson, mZoomSpeed);
	JSON_LOAD(inJson, mTargetZoom);
	JSON_LOAD(inJson, mZoom);
}

Camera::Camera(const ConstructParameters& inParameters)
	: Base(inParameters),
	mPosition(inParameters.mPosition),
	mSize(inParameters.mSize),
	mZoom(inParameters.mZoom)
{
	mSizeInverse = Vec2{1.f} / mSize;

	IVec2 size = {mSize.ToIVec2()};
	mRenderTexture = SDL_CreateTexture(gRenderer.GetRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size.mX, size.mY);
	if (!mRenderTexture)
	{
		gLog(ELogType::Error, "Error creating render target texture: %s\n", SDL_GetError());
	}
}

void Camera::Update(float inDeltaTime)
{
	mPosition = gLerp2(mPosition, mTargetPosition, inDeltaTime * mPositionSpeed);
	mZoom = gLerp(mZoom, mTargetZoom, inDeltaTime * mZoomSpeed);
}

void Camera::SetPosition(Vec2 inPosition)
{
	mTargetPosition = inPosition;
}

void Camera::SnapPosition(Vec2 inPosition)
{
	mTargetPosition = inPosition;
	mPosition = inPosition;
}

void Camera::SetSize(Vec2 inSize)
{
	mSize = inSize;
	mSizeInverse = Vec2{1.f} / mSize;

	SDL_DestroyTexture(mRenderTexture);
	const IVec2& window_size = gRenderer.GetWindowSize();
	mRenderTexture = SDL_CreateTexture(gRenderer.GetRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, window_size.mX, window_size.mY);
}

void Camera::SetZoom(float inZoom)
{
	mTargetZoom = gMax(inZoom, MIN_ZOOM);
}

void Camera::SetZoomSpeed(float inZoomSpeed)
{
	mZoomSpeed = inZoomSpeed;
}

void Camera::SnapZoom(float inZoom)
{
	mTargetZoom = gMax(inZoom, MIN_ZOOM);
	mZoom = mTargetZoom;
}

void Camera::SetRenderTexture(SDL_Texture* inRenderTexture)
{
	mRenderTexture = inRenderTexture;
}
