#include <Editor/EditorApp.h>
#include <Editor/Menus/SettingsMenu.h>
#include <imgui/imgui.h>

using namespace DC;

void SettingsMenu::Update(float inDeltaTime)
{
	if (!mIsOpen)
		return;

	ImGui::Begin("Settings", &mIsOpen);

	EditorSettings& settings = GetEditorApp().GetEditorSettings();

	
	ImGui::End();

	Base::Update(inDeltaTime);
}
