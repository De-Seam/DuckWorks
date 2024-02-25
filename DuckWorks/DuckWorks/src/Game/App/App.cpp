#include "Precomp.h"
// Own include
#include "Game/App/App.h"

// Engine includes
#include "Engine/Renderer/Renderer.h"
#include "Engine/World/World.h"
#include "Engine/Events/SDLEventManager.h"

// External includes
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

	// Create World
	mWorld = std::make_unique<World>();

	MainLoop();
	return 0;
}

void App::MainLoop()
{
	mRunning = true;
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	while (mRunning)
	{
		mDeltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(end - start).count();

		gRenderer.BeginFrame();
		Update(mDeltaTime);
		gRenderer.EndFrame();

		start = end;
		end = std::chrono::steady_clock::now();
	}
}

void App::Update(float inDeltaTime)
{
	gSDLEventManager.Update();
	mWorld->Update(inDeltaTime);
	mWorld->Render();
	gRenderer.Update(inDeltaTime);
	gLog("%f : %f", 1 / inDeltaTime, inDeltaTime);
}
