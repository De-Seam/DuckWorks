#include "Precomp.h"
// Own include
#include "Game/App/App.h"

// Engine includes
#include "Engine/Debug/DebugUIWindowManager.h"
#include "Engine/Events/SDLEventManager.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Timer/TimerManager.h"
#include "Engine/World/World.h"

// External includes
#include "Engine/Events/EventManager.h"
#include "External/SDL/SDL.h"

// Std includes
#include <fstream>

App gApp;

App::App()
{
	assert(this == &gApp);
}

App::~App()
{}

int App::Run()
{
	gLog(LogType::Info, "Initializing App");
	gLogManager.Init();

	if (mUserSettings == nullptr)
		mUserSettings = std::make_unique<BaseUserSettings>();

	LoadUserSettingsFromFile(mUserSettingsFile);

	{
		// Initialize Renderer
		Renderer::InitParams params;
		params.mWindowTitle = "DuckWorks";
		params.mWindowSize = GetUserSettings()->mWindowSize;
		params.mWindowFlags = GetUserSettings()->mWindowFlags;
		params.mRendererFlags = SDL_RENDERER_ACCELERATED;
		gRenderer.Init(params);
	}

	{
		SDLEventFunction event_function;
		event_function.mEventType = SDL_QUIT;
		event_function.mFunctionPtr = [this](const SDL_Event&) { mRunning = false; };
		gSDLEventManager.AddPersistentEventFunction(event_function);
	}

	{
		SDLEventFunction event_function;
		event_function.mEventType = SDL_WINDOWEVENT;
		event_function.mFunctionPtr = [this](const SDL_Event& inEvent)
		{
			if (inEvent.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				mUserSettings->mWindowSize.x = inEvent.window.data1;
				mUserSettings->mWindowSize.y = inEvent.window.data2;
			}
			else if (inEvent.window.event == SDL_WINDOWEVENT_MAXIMIZED)
			{
				mUserSettings->mWindowFlags |= SDL_WINDOW_MAXIMIZED;
			}
			else if (inEvent.window.event == SDL_WINDOWEVENT_RESTORED)
			{
				mUserSettings->mWindowFlags &= ~SDL_WINDOW_MAXIMIZED;
			}
		};
		gSDLEventManager.AddPersistentEventFunction(event_function);
	}

	{
		gEventManager.Init();
	}

	// Create World
	mWorld = std::make_unique<World>();

	MainLoop();

	ShutdownInternal();
	return 0;
}

void App::Stop()
{
	mRunning = false;
}

void App::LoadUserSettingsFromFile(const String& inFile)
{
	mUserSettingsFile = inFile;

	std::ifstream file(inFile);
	if (!file.is_open())
		return gLog(LogType::Warning, "Failed to open user settings file: %s. A default one will be created when the program shuts down", inFile.c_str());

	Json json = Json::parse(file);
	mUserSettings->Deserialize(json);
}

void App::SaveUserSettingsToFile(const String& inFile)
{
	mUserSettingsFile = inFile;

	std::ofstream file(inFile);
	Json json = mUserSettings->Serialize();
	file << json.dump(4);
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

	//gLog("Test");

	if (!mPaused)
	{
		gTimerManager.Update(inDeltaTime);
		mWorld->Update(inDeltaTime);
	}
	mWorld->Render(inDeltaTime);
	gDebugUIWindowManager.Update(inDeltaTime);
	gRenderer.Update(inDeltaTime);
}

void App::ShutdownInternal()
{
	gRenderer.Shutdown();

	SaveUserSettingsToFile(mUserSettingsFile);

	gLogManager.Shutdown();
}
