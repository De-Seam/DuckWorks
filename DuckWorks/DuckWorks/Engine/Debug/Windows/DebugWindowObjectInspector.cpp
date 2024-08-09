#include <Precomp.h>
#include <Engine/Debug/Windows/DebugWindowObjectInspector.h>

// Engine includes
#include <Engine/Debug/DebugManager.h>
#include <Engine/Engine.h>

// External includes
#include <External/imgui/imgui.h>

void DebugWindowObjectInspector::Update(float inDeltaTime)
{
	DebugWindow::Update(inDeltaTime);

	ImGui::Begin("Object Inspector", &mIsOpen);

	Json json = gEngine->Serialize();
	if (DebugManager::sDebugDrawJson(json, "Engine"))
		gEngine->Deserialize(json);

	ImGui::End();
}
