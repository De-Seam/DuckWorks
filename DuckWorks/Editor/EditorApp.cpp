#include <Editor/EditorApp.h>

#include <DuckCore/Math/Transform.h>

#include <Engine/Files/FileManager.h>
#include <Engine/Resources/JsonFile.h>

#include <Editor/Menus/EntitySpawnerMenu.h>
#include <Editor/Menus/OutlinerMenu.h>
#include <Editor/Menus/ResourceSelectorMenu.h>
#include <Editor/Menus/SettingsMenu.h>
#include <Editor/Menus/ViewportMenu.h>

#include <imgui/imgui.h>

using namespace DC;

void EditorApp::sRegisterRTTI() 
{
	REGISTER_RTTI(EditorMenu);
	REGISTER_RTTI(EntitySpawnerMenu);
	REGISTER_RTTI(OutlinerMenu);
	REGISTER_RTTI(ViewportMenu);
}

EditorApp::EditorApp()
{
	mEditorSettings = gMakeUnique<EditorSettings>();

	mMenus.Add(gMakeUnique<ViewportMenu>(*this));
	mMenus.Add(gMakeUnique<EntitySpawnerMenu>(*this));
	mMenus.Add(gMakeUnique<OutlinerMenu>(*this));
	mMenus.Add(gMakeUnique<SettingsMenu>(*this));
	mMenus.Add(gMakeUnique<ResourceSelectorMenu>(*this));

	Ref<JsonFile> editor_json_file = gEngine->GetManager<FileManager>().Get<JsonFile>(cEditorJsonPath);
	const Json& json = editor_json_file->GetJson();

	if (const auto menus_json = json.find("mMenus"); menus_json != json.end())
	{
		for (EditorMenu* menu : mMenus)
		{
			if (auto it = menus_json->find(menu->GetRTTI().GetClassName()); it != menus_json->end())
				menu->FromJson(*it);
		}
	}
}

EditorApp::~EditorApp()
{
	Json json;
	Json& menus_json = json["mMenus"];

	for (EditorMenu* menu : mMenus)
		menus_json[menu->GetRTTI().GetClassName()] = menu->ToJson();

	json["mEditorSettings"] = mEditorSettings->ToJson();

	gEngine->GetManager<FileManager>().WriteToFile(cEditorJsonPath, json.dump(4));
}

void EditorApp::Update(float inDeltaTime)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Menus"))
		{
			for (EditorMenu* menu : mMenus)
			{
				if (ImGui::MenuItem(menu->GetRTTI().GetClassName(), nullptr, menu->GetIsOpen()))
					menu->SetIsOpen(!menu->GetIsOpen());
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	for (EditorMenu* menu : mMenus)
		menu->Update(inDeltaTime);
}
