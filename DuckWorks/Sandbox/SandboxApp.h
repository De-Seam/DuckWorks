#pragma once
#include <DuckCore/Containers/StaticArray.h>
#include <DuckCore/Math/Vector.h>
#include <DuckCore/RTTI/Ref.h>

#include <App/App.h>

#include <SDL/SDL_render.h>

class RenderTarget;

struct Pixel
{
	bool mFilled = false;
};

class SandboxApp : public App
{
public:
	static constexpr const int32 cWindowWidth = 1280;
	static constexpr const int32 cWindowHeight = 720;

	SandboxApp();
	virtual ~SandboxApp() override;

	virtual void Update(float inDeltaTime) override;

private:
	DC::StaticArray<Pixel, cWindowWidth * cWindowHeight> mPixels;

	DC::Ref<RenderTarget> mRenderTarget; 
	SDL_Texture* mTexture = nullptr;
	uint32 mRandomState = 5;
};