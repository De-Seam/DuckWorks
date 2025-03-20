#include <Engine/Engine.h>

#include <DuckCore/Events/EventManager.h>
#include <DuckCore/Managers/Manager.h>
#include <DuckCore/Managers/Managers.h>

#include <Engine/Debug/DebugUIManager.h>
#include <Engine/Events/Events.h>
#include <Engine/Events/SDLEventManager.h>
#include <Engine/Files/FileManager.h>
#include <Engine/Objects/ObjectManager.h>
#include <Engine/Resources/JsonFile.h>

using namespace DC;

Engine* gEngine = nullptr;

void Engine::sStartup()
{
	gEngine = new Engine;

	Managers::sAdd(new FileManager);
	Get<FileManager>().RegisterFileExtension<JsonFile>("json");

	Managers::sAdd(new SDLEventManager);
	Managers::sAdd(new DebugUIManager);
}

Engine::Engine()
{
	gAssert(gEngine == nullptr);
	gEngine = this;
}

Engine::~Engine()
{
	gAssert(gEngine == this);

	gEngine = nullptr;
}

void Engine::Update(float aDeltaTime)
{
	EventManager& event_manager = Get<EventManager>();

	EnginePreUpdateEvent engine_pre_update_event;
	event_manager.SendEvent(engine_pre_update_event);

	EngineUpdateEvent engine_update_event(aDeltaTime);
	event_manager.SendEvent(engine_update_event);

	EnginePostUpdateEvent engine_post_update_event;
	event_manager.SendEvent(engine_post_update_event);
}
