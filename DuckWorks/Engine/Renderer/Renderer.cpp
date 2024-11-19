#include <Engine/Renderer/Renderer.h>

// Core includes
#include <DuckCore/Math/Transform.h>

#include <Engine/Renderer/Sprite.h>
#include <Engine/Renderer/TextureResource.h>

using namespace DC;

Renderer::Renderer()
{
	mWindow = SDL_CreateWindow("DuckWorks", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);

	if (mWindow == nullptr)
	{
		gLog(LogLevel::Error, "Failed to create window\n");
		gAssert(false);
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
	if (mRenderer == nullptr)
	{
		gLog(LogLevel::Error, "Failed to create renderer\n");
		gAssert(false);
	}
}

Renderer::~Renderer()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
}

void Renderer::Init()
{
	Base::Init();
}

void Renderer::Shutdown()
{
	Base::Shutdown();
}

void Renderer::BeginFrame()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);
}

void Renderer::EndFrame()
{
	SDL_RenderPresent(mRenderer);
}

void Renderer::DrawSprite(const Sprite& inSprite, const Transform2D& inTransform)
{
	SDL_Rect src_rect;
	src_rect.x = inSprite.GetBottomLeft().mX;
	src_rect.y = inSprite.GetBottomLeft().mY;
	src_rect.w = inSprite.GetTopRight().mX - inSprite.GetBottomLeft().mX;
	src_rect.h = inSprite.GetTopRight().mY - inSprite.GetBottomLeft().mY;

	SDL_FRect dest_rect;
	dest_rect.x = inTransform.mPosition.mX;
	dest_rect.y = inTransform.mPosition.mY;
	dest_rect.w = inTransform.mHalfSize.mX * 2;
	dest_rect.h = inTransform.mHalfSize.mY * 2;

	SDL_RenderCopyExF(mRenderer, inSprite.GetTextureResource().GetTexture(), &src_rect, &dest_rect, inTransform.mRotation, nullptr, SDL_FLIP_NONE);
}
