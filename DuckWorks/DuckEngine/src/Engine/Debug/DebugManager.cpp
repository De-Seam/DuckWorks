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

void DebugManager::Init()
{
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
		mDebugWindows.reserve(sDebugWindowNames.size());
		for (String& name : sDebugWindowNames)
			mDebugWindows.emplace_back(gEngineModule->mRTTIFactory.NewInstance<DebugWindow>(name));
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
				bool was_open = mDebugWindows[i]->IsOpen();
				bool is_open = was_open;
				ImGui::MenuItem(*menu_item_name, nullptr, &is_open);
				if (!is_open)
					mDebugWindows[i]->SetOpen(false);
				else if (!was_open)
					mDebugWindows[i]->SetOpen(true);
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
	sf::RenderWindow& render_window = gEngine->GetRenderer().GetRenderWindow();
	ImGui::SFML::ProcessEvent(render_window, inMsg.mEvent);
}

