#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindowImGuiExample.h"

#include "External/imgui/imgui.h"

void DebugUIWindowImGuiExample::Update(float inDeltaTime)
{
	ImGui::ShowDemoWindow(&mOpen);
}
