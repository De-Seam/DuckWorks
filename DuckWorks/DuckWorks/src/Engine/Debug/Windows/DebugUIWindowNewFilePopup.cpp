#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindowNewFilePopup.h"

// Engine includes
#include "Engine/Debug/DebugUIWindowManager.h"
#include "Engine/Debug/Windows/DebugUIWindowEditorToolbar.h"

// Game includes
#include "Game/App/App.h"

// External includes
#include <External/imgui/imgui.h>

// Std includes
#include <algorithm>

RTTI_CLASS_DEFINITION(DebugUIWindowNewFilePopup, StandardAllocator)

Json DebugUIWindowNewFilePopup::Serialize()
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mPath);
	json["mOpen"] = false;

	return json;
}

void DebugUIWindowNewFilePopup::Deserialize(const Json &inJson)
{
	Base::Deserialize(inJson);

	JSON_LOAD(inJson, mPath);
}

void DebugUIWindowNewFilePopup::UpdateMultiThreaded(float) {}

void DebugUIWindowNewFilePopup::Update(float) 
{
	if (ImGui::Button("World##NewFileWorldButton"))
	{
		gDebugUIWindowManager.GetWindow<DebugUIWindowEditorToolbar>()->StopPlay();
		gApp.CreateNewEmptyWorld();
	}
}

void DebugUIWindowNewFilePopup::SetPath(const String &inPath) {
	mPath = inPath;
}
