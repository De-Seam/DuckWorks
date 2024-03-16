#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindowEntitySpawner.h"

// Engine includes
#include "Engine/Factory/Factory.h"
#include "Engine/World/World.h"
#include "Engine/Renderer/Renderer.h"

// Game includes
#include "Game/App/App.h"

// External includes
#include "External/imgui/imgui.h"

RTTI_EMPTY_SERIALIZE_DEFINITION(DebugUIWindowEntitySpawner)

DebugUIWindowEntitySpawner::DebugUIWindowEntitySpawner()
{}

void DebugUIWindowEntitySpawner::Update(float inDeltaTime)
{
	(void)inDeltaTime;
	PROFILE_SCOPE(DebugUIWindowEntitySpawner::Update)

	ImGui::Begin("Entity Spawner", &mOpen);

	const Array<String>& entity_names = gEntityFactory.GetClassNames();

	for (const String& entity_name : entity_names)
	{
		if (ImGui::Button(entity_name.c_str()))
		{
			SharedPtr<Entity> entity = gEntityFactory.CreateClass(entity_name, gApp.GetWorld());
			gApp.GetWorld()->AddEntity(entity, entity_name);
			if (entity->HasComponent<TransformComponent>())
			{
				TransformComponent& transform_component = entity->GetComponent<TransformComponent>();
				transform_component.mTransform.position = gRenderer.GetCamera()->GetPosition();
			}
		}
	}

	ImGui::End();
}
