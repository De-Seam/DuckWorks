#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindowEntityDetails.h"

// Engine includes
#include "Engine/Debug/DebugUIFunctions.h"
#include "Engine/Debug/DebugUIWindowManager.h"
#include "Engine/Entity/Entity.h"
#include "Engine/Entity/Components.h"
#include "Engine/Factory/Factory.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/World/World.h"

// External includes
#include <External/imgui/imgui.h>
#include <Engine/Entity/CollisionActor.h>


RTTI_CLASS_DEFINITION(DebugUIWindowEntityDetails)

RTTI_EMPTY_SERIALIZE_DEFINITION(DebugUIWindowEntityDetails)

void DebugUIWindowEntityDetails::Update(float inDeltaTime)
{
	(void)inDeltaTime;
	PROFILE_SCOPE(DebugUIWindowEntityDetails::Update)

	Optional<WeakRef<Entity>> selected_entity_weak_ref = gDebugUIWindowManager.GetSelectedEntity();
	if (!selected_entity_weak_ref.has_value() || !selected_entity_weak_ref->IsAlive())
	{
		mOpen = false;
		return;
	}

	Ref<Entity> selected_entity = selected_entity_weak_ref.value().Get();
	gAssert(selected_entity.IsValid(), "Somehow the selected entity was invalid!");

	if (!ImGui::Begin("Entity Details", &mOpen))
	{
		ImGui::End();
		return;
	}

	ImGui::Text("%s", selected_entity->GetName().c_str());

	ImGui::SameLine();

	if (ImGui::Button("Destroy##DeleteButton"))
		selected_entity->Destroy();

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

	const Array<String>& all_component_names = gEntityComponentFactory.GetClassNames();
	for (const String& component_name : all_component_names)
	{
		UID component_type_uid = gEntityComponentFactory.GetRTTIUID(component_name);
		bool has_component = selected_entity->HasComponent(component_type_uid);

		if (!has_component)
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.4f, 0.4f, 1.f));

		if (ImGui::TreeNode((component_name + "##TreeNode").c_str()))
		{
			if (has_component)
			{
				UID component_rtti_uid = gEntityComponentFactory.GetRTTIUID(component_name);
				Array<EntityComponent*> components = selected_entity->GetComponentsOfType(component_rtti_uid);

				for (uint64 i = 0; i < components.size(); i++)
				{
					EntityComponent* component = components[i];
					Json& json_component = json_components[i];

					bool changed = gDebugDrawJson(json_component, component_name);
					if (changed)
						component->Deserialize(json_component);
				}
			}
			else
			{
				ImGui::PopStyleColor(1);
				String label = String("+##" + component_name + "AddButton");
				if (ImGui::SmallButton(label.c_str()))
					gEntityComponentFactory.AddComponent(selected_entity, component_name);
			}
			ImGui::TreePop();
		}
		else if (!has_component)
			ImGui::PopStyleColor(1);
	}

	if (gDebugUIWindowManager.mDrawEntityOutline)
	{
		SDL_FRect rect;
		fm::Transform2D transform = selected_entity->GetTransform();
		rect = gRenderer.GetSDLFRect(transform.position, transform.halfSize);
		gRenderer.DrawRectangle(rect, {0.5f, 1.f, 0.5f, 0.75f});
	}

	ImGui::End();
}
