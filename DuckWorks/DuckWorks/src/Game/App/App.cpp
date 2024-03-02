#include "Precomp.h"
// Own include
#include "Game/App/App.h"

// Engine includes
#include "Engine/Renderer/Renderer.h"
#include "Engine/World/World.h"
#include "Engine/Events/SDLEventManager.h"

// External includes
#include "Engine/Debug/DebugUIWindowManager.h"
#include "Engine/Debug/Windows/DebugUIWindowPerformanceMonitor.h"
#include "Engine/Timer/TimerManager.h"
#include "External/imgui/imgui.h"
#include "External/imgui/imgui_impl_sdl2.h"
#include "External/imgui/imgui_impl_sdlrenderer2.h"
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
		params.mWindowSize = fm::ivec2(1200, 720);
		params.mWindowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
		params.mRendererFlags = SDL_RENDERER_ACCELERATED;
		gRenderer.Init(params);
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(gRenderer.GetWindow(), gRenderer.GetRenderer());
	ImGui_ImplSDLRenderer2_Init(gRenderer.GetRenderer());

	{
		SDLEventFunction event_function;
		event_function.mAllEvents = true;
		event_function.mFunctionPtr = [this](const SDL_Event& inEvent) { ImGui_ImplSDL2_ProcessEvent(&inEvent); };
		gSDLEventManager.AddPersistentEventFunction(event_function);
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

	static bool show_demo_window = true;

	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);
}

void App::ShutdownInternal()
{
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	gRenderer.Shutdown();
	LogManager::Shutdown();
}
