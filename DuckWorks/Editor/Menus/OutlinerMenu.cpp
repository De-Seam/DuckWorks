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
	for (int i = 0; i < entities.Length(); i++)
	{
		ImGui::PushID(i);

		const Entity* entity = entities[i];
		String entity_label = String(entity->GetRTTI().GetClassName()) + "##" + gToString(i);
		if (ImGui::TreeNode(entity->GetRTTI().GetClassName()))
		{
			ImGui::TreePop();
		}

		ImGui::PopID();
	}

	ImGui::End();
}
