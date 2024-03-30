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

#include "Engine/World/World.h"

RTTI_CLASS_DEFINITION(DebugUIWindowEntityDetails)

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

	Json json_entity = selected_entity->Serialize();
	bool entity_changed = false;

	for (const auto& [key, value] : json_entity.items())
	{
		if (key == "Components")
			continue;

		if (gHandleKeyValuePair(json_entity, "EntityVariables##EntityDetails", key, value, false))
			entity_changed = true;
	}
	if (entity_changed)
		selected_entity->Deserialize(json_entity);

	Json& json_components = json_entity["Components"];

	ImGui::Separator();
	ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 1.f), "Components");

	const Array<String>& all_component_names = gComponentFactory.GetClassNames();
	for (const String& component_name : all_component_names)
	{
		bool has_component = json_components.contains(component_name);
		ImGui::Separator();
		ImVec4 component_text_color = has_component ? ImVec4{0.25, 1.f, 0.25f, 1.f} : ImVec4{1.f, 0.25f, 0.25f, 1.f};
		ImGui::TextColored(component_text_color, "%s", component_name.c_str());
		if (has_component)
		{
			ComponentBase* component = gComponentFactory.GetComponent(component_name, selected_entity->GetRegistry(), selected_entity->GetEntityHandle());

			Json& json = json_components[component_name];

			bool changed = gDebugDrawJson(json, component_name);
			if (changed)
			{
				if (component->GetClassName() == TransformComponent::sGetClassName())
				{
					if (selected_entity->HasComponent<CollisionComponent>())
					{
						const CollisionObjectHandle& collision_object_handle = selected_entity->GetComponent<CollisionComponent>().mCollisionObjectHandle;
						CollisionObjectWrapper collision_object = selected_entity->GetWorld()->GetCollisionWorld()->GetCollisionObject(
							collision_object_handle);
						const fm::Transform2D collision_transform = collision_object->GetTransform();
						collision_object.Unlock();
						fm::vec2 delta_position = collision_transform.position - selected_entity->GetComponent<TransformComponent>().mTransform.position;

						component->Deserialize(json);

						const fm::Transform2D& entity_transform = selected_entity->GetComponent<TransformComponent>().mTransform;
						selected_entity->GetWorld()->GetCollisionWorld()->MoveTo(collision_object_handle, entity_transform.position + delta_position);
					}
				}
				else
					component->Deserialize(json);
			}
		}
		else
		{
			String label = String("+##" + component_name + "AddButton");
			if (ImGui::SmallButton(label.c_str()))
			{
				gComponentFactory.CreateComponent(component_name, selected_entity->GetRegistry(), selected_entity->GetEntityHandle());
			}
		}
	}

	if (gDebugUIWindowManager.mDrawEntityOutline && selected_entity->HasComponent<TransformComponent>())
	{
		TransformComponent& transform_component = selected_entity->GetComponent<TransformComponent>();
		SDL_FRect rect = gRenderer.GetSDLFRect(transform_component.mTransform.position, transform_component.mTransform.halfSize);
		gRenderer.DrawRectangle(rect, {0.5f, 1.f, 0.5f, 0.75f});
	}

	ImGui::End();
}
