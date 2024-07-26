#include "Engine/Precomp.h"
#include <Engine/Engine.h>

// Renderer includes
#include <Renderer/Renderer.h>
#include <Renderer/WindowEvents/WindowEventManager.h>
#include <External/SFML/Graphics/CircleShape.hpp>

THREADLOCAL UniquePtr<Engine> gEngine;

void Engine::Init()
{
	RegisterManagers();

	for (Manager* manager : mManagers)
		manager->Init();
}

void Engine::Shutdown()
{
	for (Manager* manager : mManagers)
		manager->Shutdown();
}

void Engine::Update(float inDeltaTime)
{
	mRenderer.BeginFrame();

	for (Manager* manager : mManagersToUpdate)
		manager->Update(inDeltaTime);

	mRenderer.Update(inDeltaTime);

	mRenderer.EndFrame();
}

void Engine::RegisterManager(Manager* inManager) 
{
	int32 type_id = inManager->GetManagerTypeID();
	if (type_id >= mManagers.size())
		mManagers.resize(type_id + 1);
	mManagers[type_id] = inManager;
	if (inManager->GetManagerSettings().mWantsUpdate)
		mManagersToUpdate.push_back(inManager);
}

void Engine::RegisterManagers()
{	
	RegisterManager(&mRenderer.GetWindowEventManager());
}
