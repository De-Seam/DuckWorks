#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindowEntitySpawner.h"

// Engine includes
#include "Engine/Factory/Factory.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/World/World.h"

// Game includes
#include "Game/App/App.h"

// External includes
#include "External/imgui/imgui.h"

RTTI_CLASS_DEFINITION(DebugUIWindowEntitySpawner)

RTTI_EMPTY_SERIALIZE_DEFINITION(DebugUIWindowEntitySpawner)

DebugUIWindowEntitySpawner::DebugUIWindowEntitySpawner() {}

void DebugUIWindowEntitySpawner::Update(float inDeltaTime)
{
	(void)inDeltaTime;
	PROFILE_SCOPE(DebugUIWindowEntitySpawner::Update)

	if(!ImGui::Begin("Entity Spawner", &mOpen))
	{
		ImGui::End();
		return;
	}

	const Array<String>& entity_names = gEntityFactory.GetClassNames();

	for (const String& entity_name : entity_names)
	{
		if (ImGui::Button(entity_name.c_str()))
		{
			Ref<Entity> entity = gEntityFactory.CreateClass(entity_name);
			gApp.GetWorld()->AddEntity(entity, entity_name);
			fm::vec2 position = gRenderer.GetCamera()->GetPosition();
			if (entity->HasComponent<TransformComponent>())
			{
				MutexReadProtectedPtr<TransformComponent> transform_component = entity->GetComponent<TransformComponent>();
				transform_component->mTransform.position = position;
			}
			if (entity->HasComponent<CollisionComponent>())
			{
				MutexReadProtectedPtr<CollisionComponent> collision_component = entity->GetComponent<CollisionComponent>();
				gApp.GetWorld()->GetCollisionWorld()->TeleportPosition(collision_component->mCollisionObjectHandle, position);
			}
		}
	}

	ImGui::End();
}
