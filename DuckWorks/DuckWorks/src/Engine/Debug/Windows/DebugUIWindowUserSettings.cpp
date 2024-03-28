#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindowUserSettings.h"
#include "Engine/Debug/DebugUIFunctions.h"

// Game includes
#include "Game/App/App.h"

// External includes
#include <External/imgui/imgui.h>

RTTI_CLASS_DECLARATION(DebugUIWindowUserSettings)

RTTI_EMPTY_SERIALIZE_DEFINITION(DebugUIWindowUserSettings)

void DebugUIWindowUserSettings::Update(float)
{
	PROFILE_SCOPE(DebugUIWindowUserSettings::Update)

	ImGui::Begin("User Settings", &mOpen);

	Json json = gApp.GetUserSettings()->Serialize();
	if (gDebugDrawJson(json, "UserSettings"))
		gApp.GetUserSettings()->Deserialize(json);

	ImGui::End();
}
