#include <Engine/Renderer/Renderer.h>

using namespace DC;

Renderer::Renderer()
{
	mWindow = SDL_CreateWindow("DuckWorks", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
	if (mWindow == nullptr)
	{
		gLog(LogLevel::Error, "Failed to create window\n");
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
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
