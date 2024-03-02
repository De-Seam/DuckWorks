#include "Precomp.h"
#include "Engine/Debug/DebugUIWindowManager.h"

// Engine includes
#include "Engine/Debug/Windows/DebugUIWindow.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Events/SDLEventManager.h"

// External includes
#include "External/imgui/imgui.h"
#include "External/imgui/imgui_impl_sdl2.h"
#include "External/imgui/imgui_impl_sdlrenderer2.h"

DebugUIWindowManager gDebugUIWindowManager = {};

void DebugUIWindowManager::Init()
{
	PROFILE_SCOPE(DebugUIWindowManager::Init)
	gLog(LogType::Info, "Initializing down DebugUIWindowManager");

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

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(gRenderer.GetWindow(), gRenderer.GetRenderer());
	ImGui_ImplSDLRenderer2_Init(gRenderer.GetRenderer());

	{
		SDLEventFunction event_function;
		event_function.mAllEvents = true;
		event_function.mFunctionPtr = [this](const SDL_Event& inEvent) { ImGui_ImplSDL2_ProcessEvent(&inEvent); };
		gSDLEventManager.AddPersistentEventFunction(event_function);
	}
}

void DebugUIWindowManager::BeginFrame()
{
	PROFILE_SCOPE(DebugUIWindowManager::BeginFrame)

	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void DebugUIWindowManager::EndFrame()
{
	PROFILE_SCOPE(DebugUIWindowManager::EndFrame)

	ImGui::Render();
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}

void DebugUIWindowManager::Shutdown()
{
	PROFILE_SCOPE(DebugUIWindowManager::Shutdown)

	gLog(LogType::Info, "Shutting down DebugUIWindowManager");

	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void DebugUIWindowManager::Update(float inDeltaTime)
{
	PROFILE_SCOPE(DebugUIWindowManager::Update)

	Array<SharedPtr<DebugUIWindow>> windows_to_remove;
	for (SharedPtr<DebugUIWindow>& window : mWindows)
	{
		window->Update(inDeltaTime);
		if (!window->IsOpen())
			windows_to_remove.push_back(window);
	}
	for (const SharedPtr<DebugUIWindow>& window : windows_to_remove)
	{
		RemoveWindow(window->GetClassName());
	}
}

WeakPtr<DebugUIWindow> DebugUIWindowManager::AddWindow(SharedPtr<DebugUIWindow> inWindow)
{
	PROFILE_SCOPE(DebugUIWindowManager::AddWindow)

	for (SharedPtr<DebugUIWindow>& window : mWindows)
	{
		if (window == inWindow || window->GetClassName() == inWindow->GetClassName())
		{
			gAssert(false, "Duplicate window already exists! If this is a mistake, check that RTTI_CLASS is defined.");
			return {};
		}
	}

	mWindows.push_back(inWindow);
	return inWindow;
}

void DebugUIWindowManager::RemoveWindow(const String& inWindowClassName)
{
	PROFILE_SCOPE(DebugUIWindowManager::RemoveWindow)

	for (uint64 i = 0; i < mWindows.size(); i++)
	{
		if (mWindows[i]->GetClassName() == inWindowClassName)
		{
			mWindows.erase(mWindows.begin() + i);
			return;
		}
	}
}
