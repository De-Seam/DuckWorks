#pragma once
#include <DuckCore/Math/Vector.h>
#include <Engine/Resources/Resource.h>
#include <imgui/imgui.h>

class EditorMenu;

class ImGuiHelpers
{
public:
	static void sDrawResourceLink(const char* inLabel, const DC::RTTI& inResourceRTTI, DC::Ref<Resource>& ioResource, EditorMenu& ioEditorMenu);

	template<typename taType>
	static bool sDraw(const char* inLabel, DC::Vec2<taType>& ioVec, taType inStep = 0, taType inStepFast = 0, const char* inFormat = nullptr, ImGuiInputTextFlags inFlags = ImGuiInputTextFlags_None);
};

template <typename taType>
bool ImGuiHelpers::sDraw(const char* inLabel, DC::Vec2<taType>& ioVec, taType inStep, taType inStepFast, const char* inFormat, ImGuiInputTextFlags inFlags)
{
	static_assert(false, "This data type is not implemented (yet)");
	return false;
}

template <>
inline bool ImGuiHelpers::sDraw(const char* inLabel, DC::Vec2<double>& ioVec, double inStep, double inStepFast, const char* inFormat, ImGuiInputTextFlags inFlags)
{
	return ImGui::InputScalarN(inLabel, ImGuiDataType_Double, &ioVec.mX, 2, (inStep > 0.0 ? &inStep : nullptr), (inStepFast > 0.0 ? &inStepFast : nullptr), inFormat, inFlags);
}

template <>
inline bool ImGuiHelpers::sDraw(const char* inLabel, DC::Vec2<float>& ioVec, float inStep, float inStepFast, const char* inFormat, ImGuiInputTextFlags inFlags)
{
	return ImGui::InputScalarN(inLabel, ImGuiDataType_Float, &ioVec.mX, 2, (inStep > 0.0 ? &inStep : nullptr), (inStepFast > 0.0 ? &inStepFast : nullptr), inFormat, inFlags);
}

template <>
inline bool ImGuiHelpers::sDraw(const char* inLabel, DC::Vec2<int32>& ioVec, int32 inStep, int32 inStepFast, const char* inFormat, ImGuiInputTextFlags inFlags)
{
	return ImGui::InputScalarN(inLabel, ImGuiDataType_S32, &ioVec.mX, 2, (inStep > 0.0 ? &inStep : nullptr), (inStepFast > 0.0 ? &inStepFast : nullptr), inFormat, inFlags);
}

template <>
inline bool ImGuiHelpers::sDraw(const char* inLabel, DC::Vec2<uint32>& ioVec, uint32 inStep, uint32 inStepFast, const char* inFormat, ImGuiInputTextFlags inFlags)
{
	return ImGui::InputScalarN(inLabel, ImGuiDataType_U32, &ioVec.mX, 2, (inStep > 0.0 ? &inStep : nullptr), (inStepFast > 0.0 ? &inStepFast : nullptr), inFormat, inFlags);
}