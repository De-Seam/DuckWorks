#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindowEntityList.h"

// Core includes
#include "Core/Utilities/Types.h"

// Engine includes
#include "Engine/Entity/Entity.h"
#include "Engine/World/World.h"
#include "Engine/Debug/DebugUIWindowManager.h"

// Game includes
#include "Game/App/App.h"

// External includes
#include "External/imgui/imgui.h"

RTTI_CLASS_DEFINITION(DebugUIWindowEntityList)

RTTI_EMPTY_SERIALIZE_DEFINITION(DebugUIWindowEntityList)

void DebugUIWindowEntityList::Update(float)
{
	PROFILE_SCOPE(DebugUIWindowEntityList::Update)

	if(!ImGui::Begin("Entity List"))
	{
		ImGui::End();
		return;
	}

	World* world = gApp.GetWorld();
	Array<Ref<Entity>>& entities = world->GetEntities();
	Optional<WeakRef<Entity>> selected_entity = gDebugUIWindowManager.GetSelectedEntity();

	ImVec2 button_size = ImVec2(ImGui::GetContentRegionAvail().x, 0);
	for (uint64 i = 0; i < entities.size(); i++)
	{
		Ref<Entity>& entity = entities[i];
		String entity_name = entity->GetName() + "##EntityButton" + std::to_string(i);

		bool change_color = selected_entity.has_value() && entity == selected_entity.value();
		if (change_color)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); // Red button
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.5f, 0.0f, 1.0f)); // Orange on hover
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.0f, 0.0f, 1.0f)); // Dark red when clicked
		}
		if (ImGui::Button(entity_name.c_str(), button_size))
		{
			gDebugUIWindowManager.SetSelectedEntity(entity);
		}
		if (change_color)
			ImGui::PopStyleColor(3); // The parameter is the number of styles we are popping
	}

	ImGui::End();
}
