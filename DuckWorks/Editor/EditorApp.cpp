#include <DuckCore/Math/Transform.h>
#include <Editor/EditorApp.h>
#include <Editor/Menus/EntitySpawnerMenu.h>
#include <Engine/Files/FileManager.h>
#include <Engine/Resources/JsonFile.h>
#include <imgui/imgui.h>

using namespace DC;

EditorApp::EditorApp()
{
	mGameRenderTarget = gEngine->GetManager<Renderer>().CreateTexture(IVec2(800, 600));

	mGameApp = gMakeUnique<GameApp>();

	mMenus.Add(gMakeUnique<EntitySpawnerMenu>(*this));

	Ref<JsonFile> editor_json_file = gEngine->GetManager<FileManager>().Get<JsonFile>(cEditorJsonPath);
	const Json& json = editor_json_file->GetJson();

	if (const auto menus_json = json.find("mMenus"); menus_json != json.end())
	{
		for (EditorMenu* menu : mMenus)
		{
			if (auto it = menus_json->find(menu->GetRTTI().GetClassName()); it != menus_json->end())
				menu->Deserialize(*it);
		}
	}
}

EditorApp::~EditorApp()
{
	Json json;
	Json& menus_json = json["mMenus"];

	for (EditorMenu* menu : mMenus)
		menus_json[menu->GetRTTI().GetClassName()] = menu->Serialize();

	gEngine->GetManager<FileManager>().WriteToFile(cEditorJsonPath, json.dump(4));
}

void EditorApp::Update(float inDeltaTime)
{
	{
		// Render the game to the game render target
		Renderer::ScopedRenderTarget scoped_render_target(mGameRenderTarget);
		mGameApp->Update(inDeltaTime);
	}
	Transform2D transform;
	transform.mHalfSize = FVec2(800, 600) / 2.0f;
	gEngine->GetManager<Renderer>().DrawTexture(mGameRenderTarget, transform);

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
