#include "Precomp.h"
#include "Engine/Debug/DebugUIWindowManager.h"

// Engine includes
#include <Engine/Entity/Actor.h>
#include "Engine/Debug/DebugUIFunctions.h"
#include "Engine/Debug/Windows/DebugUIWindow.h"
#include "Engine/Debug/Windows/DebugUIWindowEntityDetails.h"
#include "Engine/Engine/Engine.h"
#include "Engine/Entity/Components.h"
#include "Engine/Entity/Entity.h"
#include "Engine/Events/EventManager.h"
#include "Engine/Events/SDLEventManager.h"
#include "Engine/Factory/Factory.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Threads/ThreadManager.h"
#include "Engine/World/World.h"

// External includes
#include "External/imgui/imgui.h"
#include "External/imgui/imgui_impl_sdl2.h"
#include "External/imgui/imgui_impl_sdlrenderer2.h"

// Std includes
#include <fstream>

RTTI_CLASS_DEFINITION(DebugUIWindowManager, StandardAllocator)

DebugUIWindowManager gDebugUIWindowManager = {};

Json DebugUIWindowManager::Serialize()
{
	Json json = Base::Serialize();
	JSON_SAVE(json, mDrawEntityOutline);
	JSON_SAVE(json, mDrawSelectedEntityPhysicsOutline);
	JSON_SAVE(json, mDrawCollision);
	JSON_SAVE(json, mDrawBVH);

	JSON_SAVE(json, mDebugFileName);

	// Serialize all open windows to the mSavedWindowSettings map
	for (const Ref<DebugUIWindow>& window : mWindows)
		mSavedWindowSettings[window->GetRTTIUID()] = window->Serialize();

	// Add all the windows settings to the json
	for (const auto& it : mSavedWindowSettings)
		json["Windows"].emplace_back(it.second);

	return json;
}

void DebugUIWindowManager::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	JSON_TRY_LOAD(inJson, mDrawEntityOutline);
	JSON_TRY_LOAD(inJson, mDrawSelectedEntityPhysicsOutline);
	JSON_TRY_LOAD(inJson, mDrawCollision);
	JSON_TRY_LOAD(inJson, mDrawBVH);

	JSON_TRY_LOAD(inJson, mDebugFileName);

	if (inJson.contains("Windows"))
	{
		for (const Json& json_window : inJson["Windows"])
		{
			const String& class_name = json_window["ClassName"];
			UID rtti_uid = gDebugUIWindowFactory.GetRTTIUID(class_name);
			mSavedWindowSettings[rtti_uid] = json_window;

			if (!json_window.contains("mOpen") || json_window["mOpen"] == false)
				continue;

			Ref<DebugUIWindow> window = gDebugUIWindowFactory.CreateClassFromJsonAndDeserialize(json_window);

			AddWindow(window);
		}
	}
}

void DebugUIWindowManager::Init()
{
	PROFILE_SCOPE(DebugUIWindowManager::Init)
	gLog(ELogType::Info, "Initializing DebugUIWindowManager");

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
	ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = 0.4f;
	ImGui::GetStyle().IndentSpacing = 14.f;

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
		EventManager::EventFunction event_function;
		event_function.mEventType = EventType::KeyDown;
		event_function.mFunctionPtr = [this](const EventManager::EventData& inEventData)
		{
			if (inEventData.mKeyDown.mKeyCode == KeyCode::Delete)
			{
				if (mSelectedEntity.has_value() && mSelectedEntity.value().IsAlive())
					mSelectedEntity.value().Get()->Destroy();
			}
		};
		gEventManager.AddPersistentEventFunction(event_function);
	}
	{
		EventManager::EventFunction event_function;
		event_function.mEventType = EventType::MouseButtonDown;
		event_function.mFunctionPtr = [this](const EventManager::EventData& inEventData)
		{
			if (inEventData.mMouseDown.mMouseButton == MouseButton::Left)
			{
				if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
					return;

				fm::vec2 world_location = gRenderer.GetWorldLocationAtWindowLocation(inEventData.mMouseDown.mMousePosition);
				Optional<Ref<Entity>> entity = gEngine.GetWorld()->GetEntityAtLocationSlow(world_location);
				SetSelectedEntity(entity);

				if (entity.has_value())
					mSelectedEntityRelativeLocation = entity.value()->GetPosition() - world_location;
			}
		};
		gEventManager.AddPersistentEventFunction(event_function);
	}

	std::ifstream file(mDebugFileName);
	if (file.is_open())
	{
		Json json_debug_file = Json::parse(file);
		Deserialize(json_debug_file);
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

	SDL_SetRenderTarget(gRenderer.GetRenderer(), nullptr);
}

void DebugUIWindowManager::Shutdown()
{
	PROFILE_SCOPE(DebugUIWindowManager::Shutdown)
	gLog(ELogType::Info, "Shutting down DebugUIWindowManager");

	mWindowsToAdd.clear();
	mWindows.clear();

	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	Json json_debug_file = Serialize();
	std::ofstream file(mDebugFileName);
	if (!file.is_open())
		return gLog(ELogType::Error, "Failed to open debug file for writing");

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

	for (Ref<DebugUIWindow>& window : mWindowsToAdd)
	{
		mWindows.push_back(window);
	}
	mWindowsToAdd.clear();

	UpdateViewport();

	UpdateSelectedEntity();

	UpdateMainMenuBar();

	UpdateWindows(inDeltaTime);
}

void DebugUIWindowManager::UpdateMainMenuBar()
{
	PROFILE_SCOPE(DebugUIWindowManager::UpdateMainMenuBar)

	if (ImGui::BeginMainMenuBar())
	{
		UpdateMainMenuBarWindowMenu();
		UpdateMainMenuBarDrawModes();

		ImGui::EndMainMenuBar();
	}
}

void DebugUIWindowManager::UpdateMainMenuBarWindowMenu()
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
			for (Ref<DebugUIWindow>& window : mWindows)
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
			Ref<DebugUIWindow> window = gDebugUIWindowFactory.CreateClass(window_name);
			AddWindow(window);
		}

		ImGui::EndMenu();
	}
}

void DebugUIWindowManager::UpdateMainMenuBarDrawModes()
{
	if (ImGui::BeginMenu("Draw Modes##WindowsMenu"))
	{
		ImGui::MenuItem("Draw Entity Outline##mDrawSelectedEntityPhysicsOutlineMenuItem", nullptr, &mDrawEntityOutline);
		ImGui::MenuItem("Draw Physics Outline##mDrawEntityOutlineMenuItem", nullptr, &mDrawSelectedEntityPhysicsOutline);
		ImGui::MenuItem("Draw Collision##mDrawCollisionMenuItem", nullptr, &mDrawCollision);
		ImGui::MenuItem("Draw BVH##mDrawCollisionMenuItem", nullptr, &mDrawBVH);

		ImGui::EndMenu();
	}
}

void DebugUIWindowManager::UpdateViewport()
{
	PROFILE_SCOPE(DebugUIWindowManager::UpdateViewport)

	if (mDrawBVH)
	{
		gEngine.GetWorld()->GetCollisionWorld()->DrawBVH();
	}

	if (mDrawCollision)
	{
		gEngine.GetWorld()->GetCollisionWorld()->DrawCollision();
	}
}

class DebugWindowUpdateTask : public ThreadTask
{
public:
	virtual void Execute() override
	{
		if (!mWindow.IsAlive())
			return;
		Ref<DebugUIWindow> window = mWindow.Get();
		window->UpdateMultiThreaded(mDeltaTime);
	}

	WeakRef<DebugUIWindow> mWindow;
	float mDeltaTime;
};

void DebugUIWindowManager::UpdateWindows(float inDeltaTime)
{
	PROFILE_SCOPE(DebugUIWindowManager::UpdateWindows)

	static Array<SharedPtr<DebugWindowUpdateTask>> window_update_tasks;
	for (uint64 i = window_update_tasks.size(); i < mWindows.size(); i++)
		window_update_tasks.push_back(std::make_shared<DebugWindowUpdateTask>());

	for (uint64 i = 0; i < mWindows.size(); i++)
	{
		Ref<DebugUIWindow>& window = mWindows[i];
		SharedPtr<DebugWindowUpdateTask> task = window_update_tasks[i];
		task->Reset();
		task->mWindow = window;
		task->mDeltaTime = inDeltaTime;

		gThreadManager.AddTask(task, ThreadPriority::High);
	}

	for (SharedPtr<DebugWindowUpdateTask>& task : window_update_tasks)
		task->WaitUntilCompleted();

	Array<Ref<DebugUIWindow>> windows_to_remove;
	for (Ref<DebugUIWindow>& window : mWindows)
	{
		window->Update(inDeltaTime);
		if (!window->IsOpen())
			windows_to_remove.push_back(window);
	}
	for (const Ref<DebugUIWindow>& window : windows_to_remove)
	{
		RemoveWindow(window->GetClassName());
	}
}

void DebugUIWindowManager::UpdateSelectedEntity()
{
	PROFILE_SCOPE(DebugUIWindowManager::UpdateSelectedEntity)

	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) ||
		!gEventManager.IsMouseButtonDown(MouseButton::Left) ||
		!mSelectedEntity.has_value() ||
		!mSelectedEntity.value().IsAlive())
		return;

	Ref<Entity> selected_entity = mSelectedEntity.value().Get();
	if (!selected_entity.IsValid())
		return;

	fm::vec2 old_world_location = gRenderer.GetWorldLocationAtWindowLocation(gEventManager.GetOldMousePosition());
	Optional<Ref<Entity>> old_entity = gEngine.GetWorld()->GetEntityAtLocationSlow(old_world_location);

	if (!old_entity.has_value() || old_entity.value() != selected_entity)
		return;

	fm::vec2 new_world_location = gRenderer.GetWorldLocationAtWindowLocation(gEventManager.GetMousePosition());

	selected_entity->SetPosition(new_world_location + mSelectedEntityRelativeLocation);
}

WeakRef<DebugUIWindow> DebugUIWindowManager::AddWindow(Ref<DebugUIWindow> inWindow)
{
	PROFILE_SCOPE(DebugUIWindowManager::AddWindow)

	if (mSavedWindowSettings.contains(inWindow->GetRTTIUID()))
	{
		inWindow->Deserialize(mSavedWindowSettings[inWindow->GetRTTIUID()]);
		inWindow->mOpen = true;
	}
	for (Ref<DebugUIWindow>& window : mWindows)
	{
		if (window == inWindow || window->GetClassName() == inWindow->GetClassName())
		{
			gAssert(false, "Duplicate window already exists! If this is a mistake, check that RTTI_CLASS is defined.");
			return inWindow;
		}
	}

	mWindowsToAdd.push_back(inWindow);
	return inWindow;
}

Ref<DebugUIWindow> DebugUIWindowManager::GetWindow(const String& inWindowClassName) const
{
	for (const Ref<DebugUIWindow>& window : mWindows)
	{
		if (window->GetClassName() == inWindowClassName)
			return window;
	}
	for (const Ref<DebugUIWindow>& window : mWindowsToAdd)
	{
		if (window->GetClassName() == inWindowClassName)
			return window;
	}
	return nullptr;
}

bool DebugUIWindowManager::WindowExists(const String& inWindowClassName) const
{
	for (const Ref<DebugUIWindow>& window : mWindows)
	{
		if (window->GetClassName() == inWindowClassName)
			return true;
	}
	return false;
}

void DebugUIWindowManager::RemoveWindow(const String& inWindowClassName)
{
	PROFILE_SCOPE(DebugUIWindowManager::RemoveWindow)

	for (uint64 i = 0; i < mWindows.size(); i++)
	{
		if (mWindows[i]->GetClassName() == inWindowClassName)
		{
			mSavedWindowSettings[mWindows[i]->GetRTTIUID()] = mWindows[i]->Serialize();
			mWindows.erase(mWindows.begin() + i);
			return;
		}
	}
}

void DebugUIWindowManager::SetSelectedEntity(const Optional<WeakRef<Entity>>& inEntity)
{
	mSelectedEntity = inEntity;
	if (!WindowExists(DebugUIWindowEntityDetails::sGetClassName()))
		CreateWindow<DebugUIWindowEntityDetails>();
}
