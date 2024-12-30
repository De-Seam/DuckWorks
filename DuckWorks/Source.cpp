// DuckCore includes
#include <DuckCore/Containers/UniquePtr.h>

// Engine includes
#include <Engine/Engine.h>

// App includes
#include <App/App.h>

// Apps includes
#include <CryptChat/CryptChatApp.h>
#include <Editor/EditorApp.h>
#include <Launcher/LauncherApp.h>
#include <Sandbox/SandboxApp.h>

// Std includes
#include <chrono>

void gMainLoop()
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
			DC::gLog(DC::LogLevel::Warning, DC::String::sFormatted("Slowing app, delta time took %f seconds", delta_time.count()));
			delta_time = std::chrono::duration<float>(0.1f);
		}

		last_time = current_time;

		gEngine->BeginFrame();

		App::sGetActiveApp()->Update(delta_time.count());

		gEngine->Update(delta_time.count());

		gEngine->EndFrame();
	}
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

	Engine engine;

	gEngine->Init();

    REGISTER_APP(LauncherApp);
    REGISTER_APP(GameApp);
    REGISTER_APP(EditorApp);
    REGISTER_APP(SandboxApp);
    REGISTER_APP(CryptChatApp);

	App::sSetActiveApp(DC::gMakeUnique<LauncherApp>());

	gMainLoop();

	App::sClearActiveApp();

	gEngine->Shutdown();

	return 0;
}
