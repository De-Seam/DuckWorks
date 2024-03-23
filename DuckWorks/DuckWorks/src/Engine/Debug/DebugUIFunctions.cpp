#include "Precomp.h"
#include "Engine/Debug/DebugUIFunctions.h"
#include "Engine/Entity/Components.h"
#include "Engine/Renderer/Renderer.h"

// External includes
#include "External/imgui/imgui.h"

bool gDebugDrawJson(Json& ioJson, const String& inLabel)
{
	PROFILE_SCOPE(gDebugDrawJson)
	bool changed = false;

	for (const auto& [key, value] : ioJson.items())
	{
		if (gHandleKeyValuePair(ioJson, inLabel, key, value, false))
			changed = true;
	}

	return changed;
}

bool gHandleKeyValuePair(Json& ioJson, const String& inLabel, const String& inKey, Json& ioValue, bool inSameLine)
{
	PROFILE_SCOPE(gHandleKeyValuePair)
	String label = String("##" + inLabel + inKey);
	nlohmann::detail::value_t value_type = ioValue.type();
	ImGui::Text(inKey.c_str());
	if (inSameLine)
		ImGui::SameLine();
	switch (value_type)
	{
	case nlohmann::detail::value_t::null:
		ImGui::Text("%s: null", inKey.c_str());
		break;
	case nlohmann::detail::value_t::object:
	{
		bool changed = false;
		for (const auto& [key, value] : ioValue.items())
		{
			if (gHandleKeyValuePair(ioJson, label, key, value, false))
				changed = true;
		}
		return changed;
	}
	break;
	case nlohmann::detail::value_t::array:
	{
		bool changed = false;
		for (const auto& [key, value] : ioValue.items())
		{
			if (gHandleKeyValuePair(ioJson, label, key, value, true))
				changed = true;
		}
		return changed;
	}
	break;
	case nlohmann::detail::value_t::string:
	{
		char buffer[STRING_BUFFER_SIZE] = {'\0'};
		String old_value = ioValue.get<std::string>();
		//std::memset(buffer, 0, STRING_BUFFER_SIZE); ///< Initialize buffer to 0
		std::memcpy(buffer, old_value.c_str(), old_value.size()); ///< Copy string to buffer
		ImGui::SetNextItemWidth(-FLT_MIN); // Make the next item span the whole width
		if (ImGui::InputText(label.c_str(), buffer, STRING_BUFFER_SIZE, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			String new_value = std::string(buffer);
			if (old_value != new_value)
			{
				ioValue = new_value;
				return true;
			}
		}
	}
	break;
	case nlohmann::detail::value_t::boolean:
	{
		bool value_bool = ioValue.get<bool>();
		if (ImGui::Checkbox(label.c_str(), &value_bool))
		{
			ioValue = value_bool;
			return true;
		}
	}
	break;
	case nlohmann::detail::value_t::number_integer:
	{
		int value_int = ioValue.get<int>();
		if (ImGui::DragInt(label.c_str(), &value_int))
		{
			ioValue = value_int;
			return true;
		}
	}
	break;
	case nlohmann::detail::value_t::number_unsigned:
	{
		int32 value_int = ioValue.get<uint32>();
		if (ImGui::DragInt(label.c_str(), &value_int))
		{
			ioValue = SCast<uint32>(value_int);
			return true;
		}
	}
	break;
	case nlohmann::detail::value_t::number_float:
	{
		float value_float = ioValue.get<float>();
		if (ImGui::DragFloat(label.c_str(), &value_float))
		{
			ioValue = value_float;
			return true;
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

	return false;
}

void gDrawAABB(const AABB& inAABB, const fm::vec4& inColor)
{
	fm::vec2 half_size = (inAABB.mMax - inAABB.mMin) * 0.5f;
	fm::vec2 position = inAABB.mMin + half_size;
	SDL_FRect rect = gRenderer.GetSDLFRect(position, half_size);
	gRenderer.DrawRectangle(rect, inColor);
}
