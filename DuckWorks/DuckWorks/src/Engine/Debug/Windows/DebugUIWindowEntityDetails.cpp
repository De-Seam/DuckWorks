#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindowEntityDetails.h"

// External includes
#include <External/imgui/imgui.h>

void DebugUIWindowEntityDetails::Update(float inDeltaTime)
{
	(void)inDeltaTime;
	PROFILE_SCOPE(DebugUIWindowEntityDetails::Update)

	ImGui::Begin("Entity Details", &mOpen);



	ImGui::End();
}
