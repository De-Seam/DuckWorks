#include <Editor/Menus/SettingsMenu.h>
#include <imgui/imgui.h>

using namespace DC;

void SettingsMenu::Update(float inDeltaTime)
{
	if (!mIsOpen)
		return;

	ImGui::Begin("Settings", &mIsOpen);

	
	ImGui::End();

	Base::Update(inDeltaTime);
}
