// DuckCore includes
#include <DuckCore/Containers/UniquePtr.h>

// Engine includes
#include <Engine/Engine.h>

// App includes
#include <App/App.h>

// Std includes
#include <chrono>

DC::UniquePtr<App> gApp;

void gMainLoop()
{
	// Initial time point
    std::chrono::time_point last_time = std::chrono::high_resolution_clock::now();

    while (!gApp->ShouldShutdown()) 
    {
        // Calculate the current time and delta time
        std::chrono::time_point current_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> delta_time = current_time - last_time;
        last_time = current_time;

        gEngine->Update(delta_time.count());
        gApp->Update(delta_time.count());
    }
}

int main()
{
    gApp = DC::UniquePtr<App>::sMakeUnique();

    gMainLoop();

    gApp.Delete();

    return 0;
}
