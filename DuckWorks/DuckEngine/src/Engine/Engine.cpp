#include "Engine/Precomp.h"
#include <Engine/Engine.h>

// Engine includes
#include <Engine/Debug/DebugManager.h>
#include <Engine/Renderer/Renderer.h>
#include <Engine/Renderer/WindowEvents/WindowEventManager.h>

// External includes
#include <External/SFML/Graphics/CircleShape.hpp>

THREADLOCAL Engine* gEngine = nullptr;

Engine::Engine() 
{
	gAssert(gEngine == nullptr && "Engine already exists");
	gEngine = this;
}

Engine::~Engine() 
{
	gAssert(gEngine == this && "Engine does not exist");
	gEngine = nullptr;
}

void Engine::Init()
{
	RegisterManagers();

	for (Manager* manager : mManagers)
		manager->Init();

	GetManager<WindowEventManager>().RegisterMessageListener(this, &Engine::OnWindowClosed);
}

void Engine::Shutdown()
{
	for (Manager* manager : mManagers)
		manager->Shutdown();

	GetManager<WindowEventManager>().UnregisterMessageListener(this, &Engine::OnWindowClosed);
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

void Engine::OnWindowClosed(const MsgWindowClosed&) 
{
	RequestShutdown();
}

void Engine::RegisterManagers()
{	
	RegisterManager(&mRenderer.GetWindowEventManager());
	CreateManager<DebugManager>();
}
