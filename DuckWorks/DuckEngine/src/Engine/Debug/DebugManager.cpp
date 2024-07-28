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
	if (!ImGui::SFML::Init(gEngine->GetRenderer().GetRenderWindow())) 
	{
		gAssert(false);
		return;
	}
	gEngine->GetManager<WindowEventManager>().RegisterMessageListener(this, &DebugManager::OnAnyWindowEvent);
}

void DebugManager::Shutdown()
{
	ImGui::SFML::Shutdown();
	gEngine->GetManager<WindowEventManager>().UnregisterMessageListener(this, &DebugManager::OnAnyWindowEvent);
}

void DebugManager::Update(float inDeltaTime)
{
	PROFILE_SCOPE(DebugManager::Update)

	sf::RenderWindow& render_window = gEngine->GetRenderer().GetRenderWindow();
	THREADLOCAL static sf::Clock delta_clock;
	ImGui::SFML::Update(render_window, delta_clock.restart());

	THREADLOCAL static Array<String> sDebugWindowNames;
	sDebugWindowNames.clear();
	gEngineModule->mRTTIFactory.GetSubClassNames<DebugWindow>(sDebugWindowNames);
	for (const String& window_name : sDebugWindowNames)
	{
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

