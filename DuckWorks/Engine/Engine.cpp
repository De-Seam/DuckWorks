#include "Engine.h"

// Engine includes
#include <Engine/Manager.h>

Engine* gEngine = nullptr;

Engine::Engine()
{
	gAssert(gEngine == nullptr);
}

Engine::~Engine()
{
	gAssert(gEngine == this);

	mManagers.Clear();
}

void Engine::Init()
{
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
	
}
