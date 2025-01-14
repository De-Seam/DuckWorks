#include <DuckCore/RTTI/RTTIRegistry.h>
#include <Editor/EditorApp.h>
#include <Editor/Menus/OutlinerMenu.h>
#include <Editor/Menus/ViewportMenu.h>
#include <Game/Entity/Entity.h>
#include <Game/Entity/EntityService.h>
#include <Game/Entity/Components/Component.h>
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
		if (ImGui::TreeNode(entity->GetRTTI().GetClassName()))
		{
			DrawEntity(world->GetService<EntityService>().GetEntity(*entity));
			ImGui::TreePop();
		}

		ImGui::PopID();
	}

	ImGui::End();
}

void OutlinerMenu::DrawEntity(Entity& inEntity)
{
	World& world = inEntity.GetWorld();

	Array<RTTI*> child_components;
	gRTTIRegistry.GetChildClassesOf<ComponentBase>(child_components);

	const HashMap<uint64, World::ComponentData>& component_type_to_data = world.GetComponentTypeToDataMap();
	component_type_to_data.ForEach([&world, &inEntity](uint64 inComponentType, const World::ComponentData& inComponentData)
	{
		ImGui::PushID(inComponentType);

		if (!world.HasComponent(inEntity.GetEntityHandle(), inComponentType))
		{
			String button_text = "Add ";
			button_text += inComponentData.mName;
			if (ImGui::Button(*button_text))
				world.AddComponent(inEntity.GetEntityHandle(), inComponentType);

			ImGui::PopID();
			return;
		}

		if (ImGui::TreeNode(*inComponentData.mName))
		{
			ImGui::TreePop();
		}

		ImGui::PopID();
	});
}
