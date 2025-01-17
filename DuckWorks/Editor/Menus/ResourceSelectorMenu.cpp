#include <Editor/Menus/ResourceSelectorMenu.h>
#include <imgui/imgui.h>

using namespace DC;

void ResourceSelectorMenu::Update(float inDeltaTime)
{
	if (!mIsOpen)
		return;

	ImGui::Begin("Select Resource", &mIsOpen);

	ImGui::ShowDemoWindow(&mIsOpen);

	ImGui::End();

	Base::Update(inDeltaTime);
}