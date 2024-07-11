#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindowImGuiExample.h"

#ifdef _DEBUG

#include "External/imgui/imgui.h"

RTTI_CLASS_DEFINITION(DebugUIWindowImGuiExample, StandardAllocator)

RTTI_EMPTY_SERIALIZE_DEFINITION(DebugUIWindowImGuiExample)

void DebugUIWindowImGuiExample::Update(float)
{
	PROFILE_SCOPE(DebugUIWindowImGuiExample::Update)

	ImGui::ShowDemoWindow(&mOpen);
}

#endif
