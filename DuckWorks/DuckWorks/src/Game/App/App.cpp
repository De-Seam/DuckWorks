#include "Precomp.h"
// Own include
#include "Game/App/App.h"

// Engine includes
#include "Engine/Renderer/Renderer.h"
#include "Engine/World/World.h"
#include "Engine/Events/SDLEventManager.h"
#include "Engine/Debug/DebugUIWindowManager.h"
#include "Engine/Debug/Windows/DebugUIWindowPerformanceMonitor.h"
#include "Engine/Timer/TimerManager.h"

// External includes
#include "External/SDL/SDL.h"

App gApp;

App::App()
{
	assert(this == &gApp);
}

App::~App()
{}

int App::Run()
{
	LogManager::Init();

	{
		// Initialize Renderer
		Renderer::InitParams params;
		params.mWindowTitle = "DuckWorks";
		params.mWindowSize = fm::ivec2(1920, 1080);
		params.mWindowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
		params.mRendererFlags = SDL_RENDERER_ACCELERATED;
		gRenderer.Init(params);
	}

	{
		SDLEventFunction event_function;
		event_function.mEventType = SDL_QUIT;
		event_function.mFunctionPtr = [this](const SDL_Event&) { mRunning = false; };
		gSDLEventManager.AddPersistentEventFunction(event_function);
	}

	// Create World
	mWorld = std::make_unique<World>();

	gDebugUIWindowManager.CreateWindow<DebugUIWindowPerformanceMonitor>();

	MainLoop();

	ShutdownInternal();
	return 0;
}

void App::Stop()
{
	mRunning = false;
}

void App::MainLoop()
{
	mRunning = true;
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	while (mRunning)
	{
		auto end = std::chrono::steady_clock::now();
		mDeltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(end - start).count();

		OPTICK_FRAME("MainThread")

		gRenderer.BeginFrame();
		Update(mDeltaTime);
		gRenderer.EndFrame();

		{
			//OPTICK_EVENT("Sleep");
			//std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}

		start = end;
	}
}

void App::Update(float inDeltaTime)
{
	OPTICK_EVENT("App::Update")

	gSDLEventManager.Update();

	if (!mPaused)
	{
		gTimerManager.Update(inDeltaTime);
		mWorld->Update(inDeltaTime);
	}
	mWorld->Render(inDeltaTime);
	gDebugUIWindowManager.Update(inDeltaTime);
	gRenderer.Update(inDeltaTime);
	gLog("%f : %f", 1 / inDeltaTime, inDeltaTime);
}

void App::ShutdownInternal()
{
	gRenderer.Shutdown();
	LogManager::Shutdown();
}
