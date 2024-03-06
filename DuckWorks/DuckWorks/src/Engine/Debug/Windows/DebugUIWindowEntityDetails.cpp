#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindowEntityDetails.h"

// Engine includes
#include "Engine/Debug/DebugUIWindowManager.h"
#include "Engine/Entity/Entity.h"
#include "Engine/Factory/Factory.h"

// External includes
#include <External/imgui/imgui.h>


void DebugUIWindowEntityDetails::Update(float inDeltaTime)
{
	(void)inDeltaTime;
	PROFILE_SCOPE(DebugUIWindowEntityDetails::Update)

	SharedPtr selected_entity = gDebugUIWindowManager.GetSelectedEntity().lock();
	if (!selected_entity)
	{
		mOpen = false;
		return;
	}

	ImGui::Begin("Entity Details", &mOpen);

	ImGui::Text("%s", selected_entity->GetName().c_str());

	const Array<String>& all_component_names = gComponentFactory.GetClassNames();
	for (const String& component_name : all_component_names)
	{
		bool has_component = gComponentFactory.HasComponent(component_name, selected_entity->GetRegistry(), selected_entity->GetEntityHandle());
		ImGui::Separator();
		ImVec4 component_text_color = has_component ? ImVec4{0.25, 1.f, 0.25f, 1.f} : ImVec4{1.f, 0.25f, 0.25f, 1.f};
		ImGui::TextColored(component_text_color, "%s", component_name.c_str());
		if (has_component)
		{
			ComponentBase* component = gComponentFactory.GetComponent(component_name, selected_entity->GetRegistry(), selected_entity->GetEntityHandle());
			Json json = component->Serialize();
			for (const auto& [key, value] : json.items())
			{
				HandleKeyValuePair(json, component_name, component, key, value);
			}
		}
		else
		{
			String label = String("+##" + component_name + "AddButton").c_str();
			if (ImGui::SmallButton(label.c_str()))
			{
				gComponentFactory.CreateComponent(component_name, selected_entity->GetRegistry(), selected_entity->GetEntityHandle());
			}
		}
	}

	ImGui::End();
}

void DebugUIWindowEntityDetails::HandleKeyValuePair(Json& inJson, const String& inComponentName, ComponentBase* inComponent, const String& inKey, Json& inValue,
													bool inSameLine)
{
	String label = String("##" + inComponentName + inKey).c_str();
	nlohmann::detail::value_t value_type = inValue.type();
	ImGui::Text(inKey.c_str());
	if (inSameLine)
		ImGui::SameLine();
	switch (value_type)
	{
	case nlohmann::detail::value_t::null:
		ImGui::Text("%s: null", inKey.c_str());
		break;
	case nlohmann::detail::value_t::object:
		ImGui::Text("%s: object", inKey.c_str());
		break;
	case nlohmann::detail::value_t::array:
	{
		for (const auto& [key2, value2] : inValue.items())
		{
			HandleKeyValuePair(inJson, inComponentName, inComponent, key2, value2, true);
		}
	}
	break;
	case nlohmann::detail::value_t::string:
	{
		char buffer[STRING_BUFFER_SIZE] = {'\0'};
		String old_value = inValue.get<std::string>();
		//std::memset(buffer, 0, STRING_BUFFER_SIZE); ///< Initialize buffer to 0
		std::memcpy(buffer, old_value.c_str(), old_value.size()); ///< Copy string to buffer
		ImGui::SetNextItemWidth(-FLT_MIN); // Make the next item span the whole width
		if (ImGui::InputText(label.c_str(), buffer, STRING_BUFFER_SIZE, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			String new_value = std::string(buffer);
			if (old_value != new_value)
			{
				inValue = new_value;
				inComponent->Deserialize(inJson);
			}
		}
	}
	break;
	case nlohmann::detail::value_t::boolean:
	{
		bool value_bool = inValue.get<bool>();
		if (ImGui::Checkbox(label.c_str(), &value_bool))
		{
			inValue = value_bool;
			inComponent->Deserialize(inJson);
		}
	}
	break;
	case nlohmann::detail::value_t::number_integer:
	{
		int value_int = inValue.get<int>();
		if (ImGui::DragInt(label.c_str(), &value_int))
		{
			inValue = value_int;
			inComponent->Deserialize(inJson);
		}
	}
	break;
	case nlohmann::detail::value_t::number_unsigned:
	{
		int32 value_int = inValue.get<uint32>();
		if (ImGui::DragInt(label.c_str(), &value_int))
		{
			inValue = SCast<uint32>(value_int);
			inComponent->Deserialize(inJson);
		}
	}
	break;
	case nlohmann::detail::value_t::number_float:
	{
		float value_float = inValue.get<float>();
		if (ImGui::DragFloat(label.c_str(), &value_float))
		{
			inValue = value_float;
			inComponent->Deserialize(inJson);
		}
	}
	break;
	case nlohmann::detail::value_t::binary:
		ImGui::Text("%s: binary", inKey.c_str());
		break;
	case nlohmann::detail::value_t::discarded:
		ImGui::Text("%s: discarded", inKey.c_str());
		break;
	}
}
