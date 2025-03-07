#include <DuckCore/Containers/UniquePtr.h>
#include <DuckCore/Core/Core.h>
#include <DuckCore/Core/Log.h>
#include <DuckCore/Events/EventManager.h>
#include <DuckCore/Managers/CommandLineArgumentsManager.h>
#include <DuckCore/Managers/Managers.h>

#include <Engine/Engine.h>
#include <Engine/Events/SDLEventManager.h>
#include <Engine/Objects/ObjectManager.h>

#include <Editor/Editor.h>

#include <App/App.h>

#include <Chess/ChessApp.h>

#include <CryptChat/CryptChatApp.h>

#include <Editor/EditorApp.h>

#include <Game/GameApp.h>

#include <Launcher/LauncherApp.h>

#include <Sandbox/SandboxApp.h>

#include <chrono>

// External
// DuckCore - Containers, Math, RTTI, etc.
// Engine - Resource, Asset, Managers, etc.
// Editor - Editor, ResourceEditor, AssetEditor
// Game - GameEditor, World, WorldEditor, Entity, EntityEditor, Component, ComponentEditor

// Multiple different editors: GameEditor, EntityEditor, AssetEditor, etc.
// "DuckEditor" is the "editor editor" E.g. the editor app, which contains and manages the other editors.

// "Game" object is the game itself. It will receive the "update" and "draw" calls.
// "World" object is the world. It will contain the scenes.
// "Scene" object is the scene. It will contain the entities.
// "Entity" object is the entity. It will contain the components.
// "Game" can have multiple instances, once for each GameEditor for example.

// Main pattern will be editor using other object pattern.
// Object will have a "GetGUID" function, which will be used to identify the object.
// Editor will then use that (virtual) function to identify the object.
// Object doesn't know about the editor. Editor does know about the object.\

// GameApp: On Editor Update Event ->  Update Game
// GameApp: On Editor Render Event -> Render Game
// EditorApp: On Editor Update Event -> Update Editor. Editor can choose to update Game.
// EditorApp: On Editor Render Event -> Render Editor. Editor can choose to render Game.

// WorldEditor: Renders world, allows editing of world. So organizing, creating, and deleting scenes. Creating world-overarching scripts, etc.
// SceneEditor: Allows editing individual scenes. Placing entities, creating world, etc.
// GameEditor: Allows playing game with tools etc enabled.

#define REGISTER_APP(inApp) App::sRegisterAppConstructor(#inApp, []() { return DC::Move(DC::MakeUnique<inApp>()); })

using namespace DC;

void MainLoop()
{
	// Initial time point
	std::chrono::time_point last_time = std::chrono::high_resolution_clock::now();

	while (!gEngine->ShouldShutdown())
	{
		// Calculate the current time and delta time
		std::chrono::time_point current_time = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> delta_time = current_time - last_time;

		// Max 100 ms, or we start slowing down the application (for debugging etc)
		if (delta_time.count() > 0.1f)
		{
			Log(ELogLevel::Info, String::sFormatted("Slowing app, delta time took %f seconds", delta_time.count()));
			delta_time = std::chrono::duration<float>(0.1f);
		}

		last_time = current_time;

		static UniquePtr<EventHandle<EngineUpdateEvent>> sEngineUpdateEventHandle = Managers::sGet<EventManager>().AddEventListener<EngineUpdateEvent>([](const EngineUpdateEvent& aEvent)
		{
			App::sGetActiveApp()->Update(aEvent.GetDeltaTime());
		});

		gEngine->Update(delta_time.count());
	}

	ShutdownEvent shutdown_event;
	Get<EventManager>().SendEvent(shutdown_event);
}

/**
Order of initialization:
GameModule constructor: Register classes
Engine constructor: Register managers
App constructor: Register managers / systems
Engine::Init: Initialize managers
**/
int main(int aArgumentCount, char* aArgumentValues[])
{
	Managers::sAdd(new CommandLineArgumentsManager(aArgumentCount, aArgumentValues));

	REGISTER_APP(LauncherApp);
	REGISTER_APP(GameApp);
	REGISTER_APP(EditorApp);
	REGISTER_APP(SandboxApp);
	REGISTER_APP(CryptChatApp);
	REGISTER_APP(ChessApp);

	{
		UniquePtr<Engine> engine = MakeUnique<Engine>();

		String app_name = "LauncherApp";
		Get<CommandLineArgumentsManager>().FindArgumentValue("-app", app_name);
		App::sSetActiveApp(app_name);

		MainLoop();

		App::sClearActiveApp();
	}

	return 0;
}
