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

void Engine::sStartup()
{
	
}

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

void Engine::Update(float aDeltaTime)
{
	EventManager& event_manager = Get<EventManager>();

	Get<Renderer>().BeginFrame();

	EnginePreUpdateEvent engine_pre_update_event;
	event_manager.SendEvent(engine_pre_update_event);

	EngineUpdateEvent engine_update_event(aDeltaTime);
	event_manager.SendEvent(engine_update_event);

	EnginePostUpdateEvent engine_post_update_event;
	event_manager.SendEvent(engine_post_update_event);

	EngineRenderEvent engine_render_event;
	event_manager.SendEvent(engine_render_event);

	Get<Renderer>().EndFrame();
}
