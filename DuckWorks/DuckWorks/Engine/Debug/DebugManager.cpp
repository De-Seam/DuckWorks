#include <Precomp.h>
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

bool DebugManager::sDebugDrawJson(Json& ioJson, const String& inLabel) 
{
	PROFILE_SCOPE(gDebugDrawJson)
	bool changed = false;

	for (const auto& [key, value] : ioJson.items())
	{
		if (sHandleKeyValuePair(ioJson, inLabel, key, value, false))
			changed = true;
	}

	return changed;
}

bool DebugManager::sHandleKeyValuePair(Json& ioJson, const String& inLabel, const String& inKey, Json& ioValue, bool inSameLine, bool inShowKey) 
{
	PROFILE_SCOPE(gHandleKeyValuePair)

	static HashMap<String, bool> sIgnoreKeys =
	{
		{"ClassName", true}
	};

	if (sIgnoreKeys.contains(inKey))
		return false;

	String label = String("##") + inLabel + inKey;
	nlohmann::detail::value_t value_type = ioValue.type();

	if (inKey == "mGUID")
	{
		String value = ioValue.get<String>();
		ImGui::TextColored(ImVec4(0.3f, 0.5f, 1.f, 1.f), *value);
		return false;
	}

	switch (value_type)
	{
	case nlohmann::detail::value_t::null:
		ImGui::Text("%s: null", *inKey);
		break;
	case nlohmann::detail::value_t::object:
	{
		if (ImGui::TreeNodeEx(*String(inKey + label + "##TreeNode"), ImGuiTreeNodeFlags_DefaultOpen))
		{
			bool changed = false;
			for (const auto& [key, value] : ioValue.items())
			{
				if (sIgnoreKeys.contains(key))
					continue;

				if (sHandleKeyValuePair(ioJson, label, key, value, false))
					changed = true;
			}
			ImGui::TreePop();
			return changed;
		}
		return false;
	}
	break;
	case nlohmann::detail::value_t::array:
	{
		if (inShowKey)
		{
			ImGui::Text(*inKey);
			if (inSameLine)
				ImGui::SameLine();
		}

		bool changed = false;
		for (const auto& [key, value] : ioValue.items())
		{
			// If the array element is an object with a ClassName display the ClassName instead of the key (index)
			const String& new_key = (value.type() == nlohmann::detail::value_t::object && value.contains("ClassName")) ? value["ClassName"].get<String>() : key;
			if (sHandleKeyValuePair(ioJson, label, new_key, value, true, false))
				changed = true;
		}
		return changed;
	}
	break;
	case nlohmann::detail::value_t::string:
	{
		if (inShowKey)
		{
			ImGui::Text(*inKey);
			if (inSameLine)
				ImGui::SameLine();
		}

		String value = ioValue.get<String>();
		char buffer[STRING_BUFFER_SIZE] = {'\0'};
		//std::memset(buffer, 0, STRING_BUFFER_SIZE); ///< Initialize buffer to 0
		std::memcpy(buffer, *value, value.Length()); ///< Copy string to buffer
		ImGui::SetNextItemWidth(-FLT_MIN); // Make the next item span the whole width
		if (ImGui::InputText(*label, buffer, STRING_BUFFER_SIZE, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			String new_value = std::string(buffer);
			if (value != new_value)
			{
				ioValue = new_value;
				return true;
			}
		}
	}
	break;
	case nlohmann::detail::value_t::boolean:
	{
		if (inShowKey)
		{
			ImGui::Text(*inKey);
			if (inSameLine)
				ImGui::SameLine();
		}

		bool value_bool = ioValue.get<bool>();
		if (ImGui::Checkbox(*label, &value_bool))
		{
			ioValue = value_bool;
			return true;
		}
	}
	break;
	case nlohmann::detail::value_t::number_integer:
	{
		if (inShowKey)
		{
			ImGui::Text(*inKey);
			if (inSameLine)
				ImGui::SameLine();
		}

		int value_int = ioValue.get<int>();
		if (ImGui::DragInt(*label, &value_int))
		{
			ioValue = value_int;
			return true;
		}
	}
	break;
	case nlohmann::detail::value_t::number_unsigned:
	{
		if (inShowKey)
		{
			ImGui::Text(*inKey);
			if (inSameLine)
				ImGui::SameLine();
		}

		int32 value_int = ioValue.get<uint32>();
		if (ImGui::DragInt(*label, &value_int))
		{
			ioValue = static_cast<uint32>(value_int);
			return true;
		}
	}
	break;
	case nlohmann::detail::value_t::number_float:
	{
		if (inShowKey)
		{
			ImGui::Text(*inKey);
			if (inSameLine)
				ImGui::SameLine();
		}

		float value_float = ioValue.get<float>();
		if (ImGui::DragFloat(*label, &value_float, 0.1f))
		{
			ioValue = value_float;
			return true;
		}
	}
	break;
	case nlohmann::detail::value_t::binary:
		ImGui::Text("%s: binary", *inKey);
		break;
	case nlohmann::detail::value_t::discarded:
		ImGui::Text("%s: discarded", *inKey);
		break;
	}

	return false;
}

void DebugManager::OnAnyWindowEvent(const MsgAnyWindowEvent& inMsg) 
{
	PROFILE_SCOPE(DebugManager::OnAnyWindowEvent)
	sf::RenderWindow& render_window = gEngine->GetRenderer().GetRenderWindow();
	ImGui::SFML::ProcessEvent(render_window, inMsg.mEvent);
}

