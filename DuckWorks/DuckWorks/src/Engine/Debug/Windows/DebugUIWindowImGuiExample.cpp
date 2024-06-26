#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindowImGuiExample.h"

#include "External/imgui/imgui.h"

RTTI_CLASS_DEFINITION(DebugUIWindowImGuiExample)

RTTI_EMPTY_SERIALIZE_DEFINITION(DebugUIWindowImGuiExample)

void DebugUIWindowImGuiExample::Update(float)
{
	PROFILE_SCOPE(DebugUIWindowImGuiExample::Update)

	ImGui::ShowDemoWindow(&mOpen);
}
