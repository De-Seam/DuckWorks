#include <Editor/Menus/EntitySpawnerMenu.h>

#include <Editor/EditorApp.h>
#include <Editor/Menus/ViewportMenu.h>

#include <Game/Entity/Entity.h>
#include <Game/Entity/EntityService.h>
#include <Game/World/World.h>

#include <imgui/imgui.h>

using namespace DC;

void EntitySpawnerMenu::Update(float inDeltaTime)
{
	if (!mIsOpen)
		return;

	ImGui::Begin("Entity Spawner", &mIsOpen);

	if (ImGui::Button("Create Entity"))
	{
		World* world = GetEditorApp().GetMenu<ViewportMenu>().GetGameApp()->GetWorld();
		Ref<Entity> entity = new Entity(*world);
		world->GetService<EntityService>().AddEntity(entity);
	}

	ImGui::End();

	Base::Update(inDeltaTime);
}
