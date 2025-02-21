#include <Editor/EditorApp.h>

#include <DuckCore/Manager/Managers.h>
#include <DuckCore/Math/Transform.h>

#include <Editor/Menus/EntitySpawnerMenu.h>
#include <Editor/Menus/OutlinerMenu.h>
#include <Editor/Menus/SettingsMenu.h>
#include <Editor/Menus/ViewportMenu.h>

#include <Engine/Files/FileManager.h>
#include <Engine/Resources/JsonFile.h>

#include <imgui/imgui.h>

using namespace DC;

EditorApp::EditorApp()
{
	mEditorSettings = MakeUnique<EditorSettings>();

	mMenus.Add(MakeUnique<ViewportMenu>(*this));
	mMenus.Add(MakeUnique<EntitySpawnerMenu>(*this));
	mMenus.Add(MakeUnique<OutlinerMenu>(*this));
	mMenus.Add(MakeUnique<SettingsMenu>(*this));

	Ref<JsonFile> editor_json_file = Managers::sGet<FileManager>().Get<JsonFile>(cEditorJsonPath);
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

	Managers::sGet<FileManager>().WriteToFile(cEditorJsonPath, json.dump(4));
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
