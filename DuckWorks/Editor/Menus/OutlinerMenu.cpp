#include <Editor/EditorApp.h>
#include <Editor/Menus/OutlinerMenu.h>
#include <Editor/Menus/ViewportMenu.h>
#include <Game/Entity/Entity.h>
#include <Game/Entity/EntityService.h>
#include <Game/World/World.h>

#include <imgui/imgui.h>

using namespace DC;

void OutlinerMenu::Update(float inDeltaTime)
{
	if (!mIsOpen)
		return;

	ImGui::Begin("Outliner", &mIsOpen);

	World* world = GetEditorApp().GetMenu<ViewportMenu>().GetGameApp()->GetWorld();
	const Array<Ref<Entity>>& entities = world->GetService<EntityService>().GetEntities();
	for (const Entity* entity : entities)
	{
		ImGui::Text(entity->GetRTTI().GetClassName());
	}

	ImGui::End();
}
