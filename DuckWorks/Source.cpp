// DuckCore includes
#include <DuckCore/Containers/UniquePtr.h>

// Engine includes
#include <Engine/Engine.h>

// Editor includes
#include <Editor/EditorApp.h>

// App includes
#include <App/App.h>

// Std includes
#include <chrono>

#include <CryptChat/CryptChatApp.h>

#include <Launcher/LauncherApp.h>

void gMainLoop()
{
	// Initial time point
    std::chrono::time_point last_time = std::chrono::high_resolution_clock::now();

    while (!gEngine->ShouldShutdown())
    {
        // Calculate the current time and delta time
        std::chrono::time_point current_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> delta_time = current_time - last_time;
        last_time = current_time;

        gEngine->BeginFrame();

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

    REGISTER_APP(LauncherApp);
    REGISTER_APP(GameApp);
    REGISTER_APP(EditorApp);
    REGISTER_APP(CryptChatApp);

	gEngine->Init();

    engine.SetApp(DC::gMakeUnique<LauncherApp>());

	gMainLoop();

	gEngine->Shutdown();

    return 0;
}
