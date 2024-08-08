#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindowWorldSettings.h"

#ifdef _DEBUG

// Engine includes
#include "Engine/Debug/DebugUIFunctions.h"
#include "Engine/Engine/Engine.h"
#include "Engine/World/World.h"

// External includes
#include <External/imgui/imgui.h>

void DebugUIWindowWorldSettings::Update(float)
{
	if (!ImGui::Begin("WorldSettings##DebugUIWindoWorldSettings", &mOpen))
	{
		ImGui::End();
		return;
	}

	World* world = gEngine.GetWorld();
	Json json = world->SerializeIgnoreEntities();
	if (gDebugDrawJson(json, "##WorldSettings"))
		world->DeserializeIgnoreEntities(json);

	ImGui::End();
}

#endif
