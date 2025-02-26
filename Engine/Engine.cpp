#include <Engine/Engine.h>

#include <DuckCore/Events/EventManager.h>
#include <DuckCore/Managers/Manager.h>
#include <DuckCore/Managers/Managers.h>

#include <Engine/Debug/DebugUIManager.h>
#include <Engine/Events/Events.h>
#include <Engine/Events/SDLEventManager.h>
#include <Engine/Files/FileManager.h>
#include <Engine/Objects/ObjectManager.h>
#include <Engine/Renderer/Renderer.h>
#include <Engine/Renderer/TextureResource.h>
#include <Engine/Resources/JsonFile.h>

using namespace DC;

Engine* gEngine = nullptr;

Engine::Engine()
{
	gAssert(gEngine == nullptr);
	gEngine = this;

	Managers::sAdd(new EventManager);
	Managers::sAdd(new FileManager);
	FileManager& file_manager = Managers::sGet<FileManager>();
	file_manager.RegisterFileExtension<JsonFile>("json");

	Managers::sAdd(new Renderer);
	Managers::sAdd(new SDLEventManager);
	Managers::sAdd(new DebugUIManager);
}

Engine::~Engine()
{
	gAssert(gEngine == this);

	gEngine = nullptr;
}

void Engine::BeginFrame()
{
	Managers::sGet<Renderer>().BeginFrame();
}

void Engine::Update(float inDeltaTime)
{
	EngineUpdateEvent engine_update_event(inDeltaTime);
	Managers::sGet<EventManager>().SendEvent(engine_update_event);
}

void Engine::EndFrame()
{
	Managers::sGet<Renderer>().EndFrame();
}
