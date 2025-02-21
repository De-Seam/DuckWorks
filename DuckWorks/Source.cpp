#include <DuckCore/Containers/UniquePtr.h>
#include <DuckCore/Core/Core.h>
#include <DuckCore/Core/Log.h>
#include <DuckCore/Events/EventManager.h>
#include <DuckCore/Manager/Managers.h>

#include <App/App.h>

#include <Chess/ChessApp.h>

#include <CryptChat/CryptChatApp.h>

#include <Editor/EditorApp.h>
#include <Editor/Menus/ViewportMenu.h>

#include <Engine/Engine.h>
#include <Engine/Events/SDLEventManager.h>
#include <Engine/Objects/ObjectManager.h>

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

		gEngine->BeginFrame();

		App::sGetActiveApp()->Update(delta_time.count());

		gEngine->Update(delta_time.count());

		gEngine->EndFrame();
	}

	ShutdownEvent shutdown_event;
	Managers::sGet<EventManager>().SendEvent(shutdown_event);
}

/**
Order of initialization:
GameModule constructor: Register classes
Engine constructor: Register managers
App constructor: Register managers / systems
Engine::Init: Initialize managers
**/
int main(int argc, char* argv[])
{
	(void)argc;
	(void)argv;

	REGISTER_APP(LauncherApp);
	REGISTER_APP(GameApp);
	REGISTER_APP(EditorApp);
	REGISTER_APP(SandboxApp);
	REGISTER_APP(CryptChatApp);
	REGISTER_APP(ChessApp);

	{
		UniquePtr<Engine> engine = MakeUnique<Engine>();

		App::sSetActiveApp(MakeUnique<LauncherApp>());

		MainLoop();

		App::sClearActiveApp();
	}

	return 0;
}
