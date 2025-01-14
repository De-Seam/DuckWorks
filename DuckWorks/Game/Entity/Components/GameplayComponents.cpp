#include <Game/Entity/Components/GameplayComponents.h>

#include <External/imgui/imgui.h>

using namespace DC;

void TransformComponent::UpdateImGui()
{
	ImGui::Text("Position: %f, %f", mTransform.mPosition.mX, mTransform.mPosition.mY);
}
