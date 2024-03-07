#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindowEntityList.h"

#include "Core/Utilities/Types.h"

#include "Engine/Entity/Entity.h"
#include "Engine/World/World.h"

#include "Game/App/App.h"

// External includes
#include "Engine/Debug/DebugUIWindowManager.h"

#include "External/imgui/imgui.h"


void DebugUIWindowEntityList::Update(float)
{
	PROFILE_SCOPE(DebugUIWindowEntityList::Update)

	ImGui::Begin("Entity List");

	World* world = gApp.GetWorld();
	Array<EntityPtr> entities = world->GetEntities();
	EntityPtr selected_entity = gDebugUIWindowManager.GetSelectedEntity().lock();

	ImVec2 button_size = ImVec2(ImGui::GetContentRegionAvail().x, 0);
	for (uint64 i = 0; i < entities.size(); i++)
	{
		EntityPtr& entity = entities[i];
		String entity_name = entity->GetName() + "##EntityButton" + std::to_string(i);

		bool change_color = entity == selected_entity;
		if (change_color)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); // Red button
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.5f, 0.0f, 1.0f)); // Orange on hover
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.0f, 0.0f, 1.0f)); // Dark red when clicked
		}
		if (ImGui::Button(entity_name.c_str(), button_size))
		{
			selected_entity = entity;
			gDebugUIWindowManager.SetSelectedEntity(entity);
		}
		if (change_color)
			ImGui::PopStyleColor(3); // The parameter is the number of styles we are popping
	}

	ImGui::End();
}
