#include <Engine/Events/SDLEventManager.h>

// Engine includes
#include <Engine/Engine.h>

// External includes
#include <External/SDL/SDL.h>

SDLEventManager::SDLEventManager()
{
	mUpdateHandle = gEngine->RegisterUpdateCallback([this](float inDeltaTime) { Update(); });

	SDL_Init(SDL_INIT_EVERYTHING);
}

SDLEventManager::~SDLEventManager()
{
	SDL_Quit();
}

void SDLEventManager::Update()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			gEngine->RequestShutdown();
			break;
		default:
			break;
		}
	}
}
