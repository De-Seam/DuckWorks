#include <Engine/Precomp.h>
#include <Engine/Debug/DebugManager.h>

// Engine includes
#include <Engine/Engine.h>
#include <Engine/EngineModule.h>
#include <Engine/Debug/Windows/DebugWindow.h>
#include <Engine/Renderer/WindowEvents/WindowEventManager.h>

// External includes
#include <External/imgui/imgui-SFML.h>
#include <External/imgui/imgui.h>
#include <External/SFML/Graphics/RenderWindow.hpp>
#include <External/SFML/System/Clock.hpp>

DebugManager::DebugManager() 
{
	mManagerSettings.mWantsUpdate = true;
}

Json DebugManager::Serialize() const
{
	Json json = Manager::Serialize();
	JSON_SAVE(json, mDebugWindows);
	return json;
}

void DebugManager::Deserialize(const Json& inJson)
{
	Manager::Deserialize(inJson);
	JSON_LOAD(inJson, mDebugWindows);
}

void DebugManager::Init()
{
	PROFILE_SCOPE(DebugManager::Init)
	IMGUI_CHECKVERSION();
	if (!ImGui::SFML::Init(gEngine->GetRenderer().GetRenderWindow())) 
	{
		gAssert(false);
		return;
	}

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = 0.4f;
	ImGui::GetStyle().IndentSpacing = 14.f;
	gEngine->GetManager<WindowEventManager>().RegisterMessageListener(this, &DebugManager::OnAnyWindowEvent);
}

void DebugManager::Shutdown()
{
	PROFILE_SCOPE(DebugManager::Shutdown)
	ImGui::SFML::Shutdown();
	gEngine->GetManager<WindowEventManager>().UnregisterMessageListener(this, &DebugManager::OnAnyWindowEvent);
}

void DebugManager::Update(float  inDeltaTime)
{
	PROFILE_SCOPE(DebugManager::Update)

	sf::RenderWindow& render_window = gEngine->GetRenderer().GetRenderWindow();
	THREADLOCAL static sf::Clock delta_clock;
	ImGui::SFML::Update(render_window, delta_clock.restart());

	ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);

	THREADLOCAL static Array<String> sDebugWindowNames;
	if (sDebugWindowNames.empty())
	{
		gEngineModule->mRTTIFactory.GetSubClassNames<DebugWindow>(sDebugWindowNames);
		mDebugWindows.resize(sDebugWindowNames.size());
		for (int32 i = 0; i < static_cast<int32>(mDebugWindows.size()); i++)
		{
			if (mDebugWindows[i] == nullptr)
			{
				mDebugWindows[i] = gEngineModule->mRTTIFactory.NewInstance<DebugWindow>(sDebugWindowNames[i]);
				gAssert(mDebugWindows[i]->GetRTTI().GetClassName() == sDebugWindowNames[i]);
			}
		}
	}

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Windows##WindowsMenu"))
		{
			for (uint64 i = 0; i < mDebugWindows.size(); i++)
			{
				String menu_item_name = sDebugWindowNames[i];
				// 11 = size of "DebugWindow"
				menu_item_name.Replace(0, 11, "");
				menu_item_name += "##MenuItem";
				bool is_open = mDebugWindows[i]->IsOpen();
				ImGui::MenuItem(*menu_item_name, nullptr, &is_open);
				mDebugWindows[i]->SetOpen(is_open);
			}

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	for (DebugWindow* window : mDebugWindows)
	{
		if (window->IsOpen())
			window->Update(inDeltaTime);
	}

	ImGui::ShowDemoWindow();
	
	ImGui::Begin("Hello, world!");
	ImGui::Button("Look at this pretty button");
	ImGui::End();
	
	ImGui::SFML::Render(render_window);
}

void DebugManager::OnAnyWindowEvent(const MsgAnyWindowEvent& inMsg) 
{
	PROFILE_SCOPE(DebugManager::OnAnyWindowEvent)
	sf::RenderWindow& render_window = gEngine->GetRenderer().GetRenderWindow();
	ImGui::SFML::ProcessEvent(render_window, inMsg.mEvent);
}

