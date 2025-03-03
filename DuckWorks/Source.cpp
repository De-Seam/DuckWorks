#include <DuckCore/Containers/UniquePtr.h>
#include <DuckCore/Core/Core.h>
#include <DuckCore/Core/Log.h>
#include <DuckCore/Events/EventManager.h>
#include <DuckCore/Managers/CommandLineArgumentsManager.h>
#include <DuckCore/Managers/Managers.h>

#include <App/App.h>

#include <Chess/ChessApp.h>

#include <CryptChat/CryptChatApp.h>

#include <Engine/Engine.h>
#include <Engine/Events/SDLEventManager.h>
#include <Engine/Objects/ObjectManager.h>

#include <Game/GameApp.h>

#include <Launcher/LauncherApp.h>

#include <Sandbox/SandboxApp.h>

#include <chrono>

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

		static UniquePtr<EventHandle<EngineUpdateEvent>> sEngineUpdateEventHandle = Managers::sGet<EventManager>().AddEventListener<EngineUpdateEvent>(nullptr, [](const EngineUpdateEvent& aEvent)
		{
			App::sGetActiveApp()->Update(aEvent.GetDeltaTime());
		});

		gEngine->Update(delta_time.count());
	}

	ShutdownEvent shutdown_event;
	Get<EventManager>().SendEvent(nullptr, shutdown_event);
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
