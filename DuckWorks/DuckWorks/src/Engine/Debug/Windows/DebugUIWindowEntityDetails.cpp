#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindowEntityDetails.h"

// Engine includes
#include "Engine/Debug/DebugUIWindowManager.h"
#include "Engine/Entity/Entity.h"
#include "Engine/Factory/Factory.h"
#include "Engine/Debug/DebugUIFunctions.h"
#include "Engine/Renderer/Renderer.h"

// External includes
#include <External/imgui/imgui.h>

RTTI_EMPTY_SERIALIZE_DEFINITION(DebugUIWindowEntityDetails)


void DebugUIWindowEntityDetails::Update(float inDeltaTime)
{
	(void)inDeltaTime;
	PROFILE_SCOPE(DebugUIWindowEntityDetails::Update)

	SharedPtr selected_entity = gDebugUIWindowManager.GetSelectedEntity().lock();
	if (!selected_entity)
	{
		mOpen = false;
		return;
	}

	ImGui::Begin("Entity Details", &mOpen);

	ImGui::Text("%s", selected_entity->GetName().c_str());

	ImGui::SameLine();

	if (ImGui::Button("Destroy##DeleteButton"))
		selected_entity->TryAddComponent<DestroyedTag>(selected_entity->mUID);

	const Array<String>& all_component_names = gComponentFactory.GetClassNames();
	for (const String& component_name : all_component_names)
	{
		bool has_component = gComponentFactory.HasComponent(component_name, selected_entity->GetRegistry(), selected_entity->GetEntityHandle());
		ImGui::Separator();
		ImVec4 component_text_color = has_component ? ImVec4{0.25, 1.f, 0.25f, 1.f} : ImVec4{1.f, 0.25f, 0.25f, 1.f};
		ImGui::TextColored(component_text_color, "%s", component_name.c_str());
		if (has_component)
		{
			ComponentBase* component = gComponentFactory.GetComponent(component_name, selected_entity->GetRegistry(), selected_entity->GetEntityHandle());
			Json json = component->Serialize();

			bool changed = gDebugDrawJson(json, component_name);
			if (changed)
				component->Deserialize(json);
		}
		else
		{
			String label = String("+##" + component_name + "AddButton").c_str();
			if (ImGui::SmallButton(label.c_str()))
			{
				gComponentFactory.CreateComponent(component_name, selected_entity->GetRegistry(), selected_entity->GetEntityHandle());
			}
		}
	}

	selected_entity->TryAddComponent<PhysicsPositionOrRotationUpdatedTag>();
	selected_entity->TryAddComponent<PhysicsSizeUpdatedTag>();

	if (gDebugUIWindowManager.mDrawEntityOutline && selected_entity->HasComponent<TransformComponent>())
	{
		TransformComponent& transform_component = selected_entity->GetComponent<TransformComponent>();
		SDL_FRect rect = gRenderer.GetSDLFRect(transform_component.mTransform.position, transform_component.mTransform.halfSize);
		gRenderer.DrawRectangle(rect, {0.5f, 1.f, 0.5f, 0.75f});
	}

	if (gDebugUIWindowManager.mDrawSelectedEntityPhysicsOutline && selected_entity->HasComponent<PhysicsComponent>())
	{
		gDrawEntityPhysicsOutline(selected_entity->GetComponent<PhysicsComponent>(), {1.f, 1.f, 1.f, 1.f});
	}

	ImGui::End();
}
