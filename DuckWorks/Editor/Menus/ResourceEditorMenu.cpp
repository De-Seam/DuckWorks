#include <Editor/Menus/ResourceEditorMenu.h>
#include <Engine/Resources/Resource.h>
#include <imgui/imgui.h>

using namespace DC;

ResourceEditorMenu::ResourceEditorMenu(EditorApp& ioEditorApp, Resource& ioResource) :
	Base(ioEditorApp),
	mResource(&ioResource)
{
	SetGUID(GUID::sCreate());
}


void ResourceEditorMenu::Update(float inDeltaTime)
{
	if (!mIsOpen)
		return;

	ImGui::PushID(*GetGUID().ToString());

	ImGui::Begin("Resource Editor", &mIsOpen);

	mResource->UpdateImGui();

	ImGui::End();

	EditorMenu::Update(inDeltaTime);

	ImGui::PopID();
}
