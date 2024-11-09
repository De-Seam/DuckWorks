#include <Engine/Renderer/Renderer.h>

using namespace DC;

Renderer::Renderer()
{
	SDL_Init(SDL_INIT_EVERYTHING);

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

	SDL_Quit();
}

void Renderer::Init()
{
	Base::Init();
}

void Renderer::Shutdown()
{
	Base::Shutdown();
}
