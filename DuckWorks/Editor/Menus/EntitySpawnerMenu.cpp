#include <Editor/Menus/EntitySpawnerMenu.h>

#include <imgui/imgui.h>

void EntitySpawnerMenu::Update(float inDeltaTime)
{
	if (!mIsOpen)
		return;

	ImGui::Begin("Entity Spawner", &mIsOpen);

	ImGui::Text("Wow!");

	ImGui::End();

	Base::Update(inDeltaTime);
}
