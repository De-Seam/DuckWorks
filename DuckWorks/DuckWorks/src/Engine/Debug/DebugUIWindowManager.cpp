#include "Precomp.h"
#include "Engine/Debug/DebugUIWindowManager.h"

// Engine includes
#include "Engine/Debug/Windows/DebugUIWindow.h"
#include "Engine/Debug/Windows/DebugUIWindowEntitySpawner.h"
#include "Engine/Debug/Windows/DebugUIWindowPerformanceMonitor.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Events/SDLEventManager.h"

// External includes
#include "Engine/Factory/Factory.h"
#include "External/imgui/imgui.h"
#include "External/imgui/imgui_impl_sdl2.h"
#include "External/imgui/imgui_impl_sdlrenderer2.h"

// Std includes
#include <fstream>

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

	std::ifstream file(mDebugFileName);
	if (file.is_open())
	{
		Json json_debug_file = Json::parse(file);
		auto open_windows = json_debug_file["OpenWindows"];
		for (String window_name : open_windows)
		{
			SharedPtr<DebugUIWindow> window = gDebugUIWindowFactory.CreateClass(window_name);
			AddWindow(window);
		}
	}
}

void DebugUIWindowManager::BeginFrame()
{
	PROFILE_SCOPE(DebugUIWindowManager::BeginFrame)

	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);
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

	Json json_debug_file;
	for (const SharedPtr<DebugUIWindow>& window : mWindows)
	{
		if (window->IsOpen())
			json_debug_file["OpenWindows"].emplace_back(window->GetClassName());
	}
	std::ofstream file(mDebugFileName);
	if (!file.is_open())
		return gLog(LogType::Error, "Failed to open debug file for writing");

	file << json_debug_file.dump(4);
}

bool replace(std::string& str, const std::string& from, const std::string& to)
{
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}

void DebugUIWindowManager::Update(float inDeltaTime)
{
	PROFILE_SCOPE(DebugUIWindowManager::Update)

	UpdateMainMenuBar();

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

void DebugUIWindowManager::UpdateMainMenuBar()
{
	PROFILE_SCOPE(DebugUIWindowManager::UpdateMainMenuBar)

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Windows##WindowsMenu"))
		{
			Array<String> remove_windows;
			Array<String> add_windows;
			const Array<String>& window_names = gDebugUIWindowFactory.GetClassNames();
			if (window_names.size() != mWindowOpen.size())
				mWindowOpen.resize(window_names.size());

			for (uint64 i = 0; i < mWindowOpen.size(); i++)
			{
				mWindowOpen[i] = false;
				for (SharedPtr<DebugUIWindow>& window : mWindows)
				{
					if (window->GetClassName() == window_names[i])
					{
						mWindowOpen[i] = true;
						break;
					}
				}
			}

			for (uint64 i = 0; i < window_names.size(); i++)
			{
				String menu_item_name = window_names[i];
				// 13 = size of "DebugUIWindow"
				menu_item_name.replace(0, 13, "");
				menu_item_name += "##MenuItem";
				bool was_open = mWindowOpen[i];
				bool is_open = was_open;
				ImGui::MenuItem(menu_item_name.c_str(), nullptr, &is_open);
				mWindowOpen[i] = is_open;

				// See if we need to add or remove this window
				if (was_open != is_open)
				{
					if (is_open)
						add_windows.emplace_back(window_names[i]);
					else // was_open == true
						remove_windows.emplace_back(window_names[i]);
				}
			}

			for (const String& window_name : remove_windows)
			{
				RemoveWindow(window_name);
			}
			for (const String& window_name : add_windows)
			{
				SharedPtr<DebugUIWindow> window = gDebugUIWindowFactory.CreateClass(window_name);
				AddWindow(window);
			}

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
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
