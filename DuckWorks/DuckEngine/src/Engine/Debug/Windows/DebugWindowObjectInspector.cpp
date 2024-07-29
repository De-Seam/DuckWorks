#include <Engine/Precomp.h>
#include <Engine/Debug/Windows/DebugWindowObjectInspector.h>

// External includes
#include <External/imgui/imgui.h>

void DebugWindowObjectInspector::Update(float inDeltaTime)
{
	DebugWindow::Update(inDeltaTime);

	ImGui::Begin("Object Inspector", &mIsOpen);
	ImGui::End();
}
