#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindowImGuiExample.h"

#include "External/imgui/imgui.h"

void DebugUIWindowImGuiExample::Update(float)
{
	ImGui::ShowDemoWindow(&mOpen);
}
