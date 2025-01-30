#include <Editor/EditorApp.h>
#include <Editor/Menus/ResourceSelectorMenu.h>
#include <Editor/Utilities/ImGuiHelper.h>

using namespace DC;

void ImGuiHelpers::sDrawResourceLink(const char* inLabel, const DC::RTTI& inResourceRTTI, Ref<Resource>& ioResource, EditorMenu& ioEditorMenu)
{
	String guid_string = ioResource->GetGUID().ToString();
	ImGui::Text(inLabel);
	ImGui::SameLine();
	if (ImGui::Button(*guid_string))
	{
		// There should only be 1 ResourceSelectorMenu active at a time, for each EditorMenu.
		ioEditorMenu.RemoveChildrenOfType<ResourceSelectorMenu>();

		ioEditorMenu.AddChild(new ResourceSelectorMenu(ioEditorMenu.GetEditorApp(), inResourceRTTI, ioResource));
	}
}
