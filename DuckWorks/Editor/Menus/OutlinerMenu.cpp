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

	for (int i = 0; i < child_components.Length(); i++)
	{
		ImGui::PushID(i);

		RTTI* component_rtti = child_components[i];

		if (world.HasComponent(inEntity.GetEntityHandle(), *component_rtti))
		{
			if (ImGui::TreeNode(component_rtti->GetClassName()))
			{
				ComponentBase* component = world.GetComponent(inEntity.GetEntityHandle(), *component_rtti);
				component->UpdateImGui();
				ImGui::TreePop();
			}
		}
		else 
		{
			String button_text = "Add ";
			button_text += component_rtti->GetClassName();
			if (ImGui::Button(*button_text))
				world.AddComponent(inEntity.GetEntityHandle(), *component_rtti);
		}

		ImGui::PopID();
	}
}
