#include <Engine/Events/SDLEventManager.h>

// Engine includes
#include <Engine/Engine.h>

// External includes
#include <External/imgui/imgui_impl_sdl2.h>
#include <External/SDL/SDL.h>

SDLEventManager::SDLEventManager()
{
	mUpdateHandle = gEngine->RegisterUpdateCallback([this](float) { Update(); });

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
		ImGui_ImplSDL2_ProcessEvent(&event);
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
