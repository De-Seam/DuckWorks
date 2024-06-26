#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindowUserSettings.h"
#include "Engine/Debug/DebugUIFunctions.h"

// Game includes
#include "Game/App/App.h"

// External includes
#include <External/imgui/imgui.h>

RTTI_CLASS_DEFINITION(DebugUIWindowUserSettings)

RTTI_EMPTY_SERIALIZE_DEFINITION(DebugUIWindowUserSettings)

void DebugUIWindowUserSettings::Update(float)
{
	PROFILE_SCOPE(DebugUIWindowUserSettings::Update)

	if(!ImGui::Begin("User Settings", &mOpen))
	{
		ImGui::End();
		return;
	}

	Json json = gApp.GetUserSettings()->Serialize();
	if (gDebugDrawJson(json, "UserSettings"))
		gApp.GetUserSettings()->Deserialize(json);

	ImGui::End();
}
