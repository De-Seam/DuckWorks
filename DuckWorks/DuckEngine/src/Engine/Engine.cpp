#include "Engine/Precomp.h"
#include <Engine/Engine.h>

// Renderer includes
#include <Renderer/Renderer.h>
#include <Renderer/WindowEvents/WindowEventManager.h>

Engine gEngine;

void Engine::Init()
{
	
}

void Engine::Shutdown()
{
	
}

void Engine::Update(float inDeltaTime)
{
	for (Manager* manager : mManagersToUpdate)
		manager->Update(inDeltaTime);
}

void Engine::RegisterManagers()
{
	CreateManager<Renderer>();
	CreateManager<WindowEventManager>();
}
