#include <Editor/Utilities/ImGuiHelper.h>
#include <Game/Entity/Components/GameplayComponents.h>

#include <External/imgui/imgui.h>

using namespace DC;

void TransformComponent::UpdateImGui(EditorMenu& ioEditorMenu)
{
	ImGui::PushID("TransformComponent");

	ImGuiHelpers::sDraw("Position", mTransform.mPosition);
	ImGuiHelpers::sDraw("Half Size", mTransform.mHalfSize);
	ImGui::InputFloat("Rotation", &mTransform.mRotation);

	ImGui::PopID();
}
