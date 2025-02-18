#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindowEntityDetails.h"

// Engine includes
#include <Engine/Entity/Actor.h>
#include "Engine/Debug/DebugUIFunctions.h"
#include "Engine/Debug/DebugUIWindowManager.h"
#include "Engine/Entity/Components.h"
#include "Engine/Entity/Entity.h"
#include "Engine/Factory/Factory.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/World/World.h"

// External includes
#include <External/imgui/imgui.h>

using namespace DC;

RTTI_CLASS_DEFINITION(DebugUIWindowEntityDetails, StandardAllocator)

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

	ImGui::Separator();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0.f, 8.f});
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{0.f, 3.f});
	if (ImGui::TreeNodeEx("Components##TreeNode", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PopStyleVar(2);

		selected_entity->LoopOverAllComponents([&](EntityComponent& inComponent)
		{
			String component_name = inComponent.GetClassName();
			Json json_component = inComponent.Serialize();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0.f, 8.f});
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{0.f, 3.f});
			if (ImGui::TreeNodeEx((component_name + "##TreeNode" + inComponent.GetGUID().ToString()).c_str(), ImGuiTreeNodeFlags_SpanFullWidth))
			{
				ImGui::PopStyleVar(2);

				bool changed = gDebugDrawJson(json_component, component_name);
				if (changed)
					inComponent.Deserialize(json_component);

				ImGui::TreePop();
			}
			else
				ImGui::PopStyleVar(2);
		});
		ImGui::TreePop();
	}
	else
		ImGui::PopStyleVar(2);

	if (gDebugUIWindowManager.mDrawEntityOutline)
	{
		if (const TextureRenderComponent* texture_render_component = selected_entity->GetFirstComponentOfType<TextureRenderComponent>())
		{
			Transform2D transform = texture_render_component->GetWorldTransform();
			Renderer::DrawRectangleParams params;
			params.mPosition = transform.mPosition;
			params.mHalfSize = transform.mHalfSize;
			params.mColor = {0.5f, 1.f, 0.5f, 0.75f};
			gRenderer.DrawRectangle(params);
		}
	}

	ImGui::End();
}
