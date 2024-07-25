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

void Engine::RegisterManager(Manager* inManager) 
{
	int32 type_id = inManager->GetManagerTypeID();
	if (type_id <= mManagers.size())
		mManagers.resize(type_id + 1);
	mManagers[type_id] = inManager;
}

void Engine::RegisterManagers()
{
	CreateManager<Renderer>();
	RegisterManager(&GetManager<WindowEventManager>());
}
