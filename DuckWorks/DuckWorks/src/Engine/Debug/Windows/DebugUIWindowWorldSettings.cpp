#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindowWorldSettings.h"

// Engine includes
#include "Engine/Debug/DebugUIFunctions.h"
#include "Engine/World/World.h"

// Game includes
#include "Game/App/App.h"

// External includes
#include <External/imgui/imgui.h>

RTTI_CLASS_DEFINITION(DebugUIWindowWorldSettings, StandardAllocator)

RTTI_EMPTY_SERIALIZE_DEFINITION(DebugUIWindowWorldSettings)

void DebugUIWindowWorldSettings::Update(float)
{
	if(!ImGui::Begin("WorldSettings##DebugUIWindoWorldSettings", &mOpen))
	{
		ImGui::End();
		return;
	}

	World* world = gApp.GetWorld();
	Json json = world->SerializeIgnoreEntities();
	if (gDebugDrawJson(json, "##WorldSettings"))
		world->Deserialize(json);

	ImGui::End();
}
