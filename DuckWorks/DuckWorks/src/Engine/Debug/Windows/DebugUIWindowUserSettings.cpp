#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindowUserSettings.h"

#ifdef _DEBUG

// Engine includes
#include "Engine/Debug/DebugUIFunctions.h"
#include "Engine/Engine/BaseUserSettings.h"
#include "Engine/Engine/Engine.h"

// External includes
#include <External/imgui/imgui.h>

RTTI_CLASS_DEFINITION(DebugUIWindowUserSettings, StandardAllocator)

RTTI_EMPTY_SERIALIZE_DEFINITION(DebugUIWindowUserSettings)

void DebugUIWindowUserSettings::Update(float)
{
	PROFILE_SCOPE(DebugUIWindowUserSettings::Update)

	if (!ImGui::Begin("User Settings", &mOpen))
	{
		ImGui::End();
		return;
	}

	Json json = gEngine.GetUserSettings()->Serialize();
	if (gDebugDrawJson(json, "UserSettings"))
		gEngine.GetUserSettings()->Deserialize(json);

	ImGui::End();
}

#endif
