#pragma once
#include "Core/Math/FMath.h"
#include "Core/Utilities/RTTI.h"

#define MIN_ZOOM 0.05f

class Entity;
struct SDL_Texture;

class Camera : public RTTIBaseClass
{
	RTTI_CLASS(Camera, RTTIBaseClass, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters
	{
		fm::vec2 mPosition = {200, 0};
		fm::vec2 mSize = {1920.f, 1080.f};
		float mZoom = 1.f;
	};

	Camera(const ConstructParameters& inParameters = {});

	virtual void Update(float inDeltaTime);
	void SetPosition(fm::vec2 inPosition);
	void SnapPosition(fm::vec2 inPosition);
	void SetSize(fm::vec2 inSize);
	void SetZoom(float inZoom);
	void SetZoomSpeed(float inZoomSpeed);
	void SnapZoom(float inZoom);

	void SetRenderTexture(SDL_Texture* inRenderTexture);

	[[nodiscard]] fm::vec2 GetPosition() const { return mPosition; }
	[[nodiscard]] float GetZoom() const { return mZoom; }
	[[nodiscard]] fm::vec2 GetSize() const { return mSize; }
	[[nodiscard]] fm::vec2 GetSizeInverse() const { return mSizeInverse; }

	[[nodiscard]] SDL_Texture* GetRenderTexture() const { return mRenderTexture; }

private:
	fm::vec2 mTargetPosition = {0.f, 0.f};
	fm::vec2 mPosition = {0.f, 0.f};
	float mPositionSpeed = 5.f;
	fm::vec2 mSize = {1920.f, 1080.f};
	fm::vec2 mSizeInverse = {1.f / 1920.f, 1.f / 1080.f};
	float mZoomSpeed = 40;
	float mTargetZoom = 1.f;
	float mZoom = 1;

	SDL_Texture* mRenderTexture = nullptr;
};
