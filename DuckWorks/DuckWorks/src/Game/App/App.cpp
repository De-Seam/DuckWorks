#include "Precomp.h"
// Own include
#include "Game/App/App.h"

// Core includes
#include "Core/Utilities/GUID.h"

// Engine includes
#include "Engine/Debug/DebugUIWindowManager.h"
#include "Engine/Debug/Windows/DebugUIWindowEditorToolbar.h"
#include "Engine/Events/EventManager.h"
#include "Engine/Events/SDLEventManager.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Resources/ResourceManager.h"
#include "Engine/Threads/ThreadManager.h"
#include "Engine/Timer/TimerManager.h"
#include "Engine/World/World.h"

// Game includes
#include "Game/App/FactoryRegistry.h"
#include "Game/Entity/Player/Player.h"

// External includes
#include "External/SDL/SDL.h"

// Std includes
#include <fstream>

App gApp;

App::App()
{
	assert(this == &gApp);
}

App::~App()
{
	assert(this == &gApp);
}

int App::Run()
{
	OPTICK_FRAME("Initialize") ///< Main thread so we can profile up until the first frame

	PROFILE_SCOPE(App::Run)

	gLog(ELogType::Info, "Initializing App");
	gLogManager.Init();

	gThreadManager.Init();

	gRegisterFactoryClasses();

	if (mUserSettings == nullptr)
		mUserSettings = std::make_unique<BaseUserSettings>();

	LoadUserSettingsFromFile(mUserSettingsFile);

	{
		// Initialize Renderer
		Renderer::InitParams params;
		params.mWindowTitle = "DuckWorks";
		params.mWindowSize = GetUserSettings()->mWindowSize;
		params.mWindowFlags = GetUserSettings()->mWindowFlags;
		params.mRendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;
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

	gEventManager.Init();

	gResourceManager.Init();

	// Create World
	mWorld = std::make_unique<World>();

	std::ifstream in_file("world.json");
	if (in_file.is_open())
	{
		Json json;
		in_file >> json;

		CreateNewWorld(json);
	}

	MainLoop();

	Ref<DebugUIWindowEditorToolbar> toolbar = gDebugUIWindowManager.GetWindow<DebugUIWindowEditorToolbar>();
	if (toolbar->GetGameState() != ToolbarGameState::Stopped)
		toolbar->StopPlay();
	toolbar->Save();
	toolbar->SaveStateToFile();

	ShutdownInternal();
	return 0;
}

void App::Stop()
{
	mRunning = false;
}

void App::LoadUserSettingsFromFile(const String& inFile)
{
	PROFILE_SCOPE(App::LoadUserSettingsFromFile)

	mUserSettingsFile = inFile;

	std::ifstream file(inFile);
	if (!file.is_open())
		return gLog(ELogType::Warning, "Failed to open user settings file: %s. A default one will be created when the program shuts down", inFile.c_str());

	Json json = Json::parse(file);
	mUserSettings->Deserialize(json);
}

void App::SaveUserSettingsToFile(const String& inFile)
{
	PROFILE_SCOPE(App::SaveUserSettingsToFile)

	mUserSettingsFile = inFile;

	std::ofstream file(inFile);
	Json json = mUserSettings->Serialize();
	file << json.dump(4);
}

void App::CreateNewWorld(const Json& inJson)
{
	if (mWorld != nullptr && mWorld->HasBegunPlay())
		mWorld->EndPlay();

	mWorld = nullptr;
	mWorld = std::make_unique<World>();
	mWorld->Deserialize(inJson);
}

void App::CreateNewEmptyWorld()
{
	if (mWorld != nullptr && mWorld->HasBegunPlay())
		mWorld->EndPlay();

	mWorld = nullptr;
	mWorld = std::make_unique<World>();
	mWorld->BeginPlay();
}

void App::MainLoop()
{
	mRunning = true;
	auto last_time = std::chrono::steady_clock::now();
	while (mRunning)
	{
		auto current_time = std::chrono::steady_clock::now();
		mDeltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(current_time - last_time).count();

		double min_delta_time = 1.0 / SCast<float>(GetUserSettings()->mMaxFPS);
		if (mDeltaTime >= min_delta_time)
		{
			OPTICK_FRAME("MainThread")

			Update(mDeltaTime);

			last_time = current_time;
		}
	}
}

void App::Update(float inDeltaTime)
{
	PROFILE_SCOPE(App::Update)

	gSDLEventManager.Update();

	if (!mPaused)
	{
		gTimerManager.Update(inDeltaTime);
		mWorld->Update(inDeltaTime);
	}
	mWorld->Render(inDeltaTime);

	// We wait until the render thread task is completed before beginning the next frame
	const SharedPtr<Renderer::RenderThreadTask>& render_thread_task = gRenderer.GetRenderThreadTask();
	if (render_thread_task != nullptr)
		render_thread_task->WaitUntilCompleted();

	gDebugUIWindowManager.BeginFrame();
	gRenderer.BeginFrame();

	gDebugUIWindowManager.Update(inDeltaTime);
	gRenderer.Update(inDeltaTime);
}

void App::ShutdownInternal()
{
	mWorld->EndPlay();
	mWorld = nullptr;

	gRenderer.Shutdown();

	SaveUserSettingsToFile(mUserSettingsFile);

	gThreadManager.Shutdown();
	gLogManager.Shutdown();
}
