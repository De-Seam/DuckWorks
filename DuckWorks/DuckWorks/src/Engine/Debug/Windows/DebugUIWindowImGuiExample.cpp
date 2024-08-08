#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindowImGuiExample.h"

#ifdef _DEBUG

#include "External/imgui/imgui.h"

void DebugUIWindowImGuiExample::Update(float)
{
	PROFILE_SCOPE(DebugUIWindowImGuiExample::Update)

	ImGui::ShowDemoWindow(&mOpen);
}

#endif
