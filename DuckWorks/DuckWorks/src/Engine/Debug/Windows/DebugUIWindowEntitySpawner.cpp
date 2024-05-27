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

RTTI_CLASS_DEFINITION(DebugUIWindowEntitySpawner, StandardAllocator)

RTTI_EMPTY_SERIALIZE_DEFINITION(DebugUIWindowEntitySpawner)

void DebugUIWindowEntitySpawner::Update(float inDeltaTime)
{
	(void)inDeltaTime;
	PROFILE_SCOPE(DebugUIWindowEntitySpawner::Update)

	if (!ImGui::Begin("Entity Spawner", &mOpen))
	{
		ImGui::End();
		return;
	}

	const Array<String>& entity_names = gEntityFactory.GetClassNames();

	for (const String& entity_name : entity_names)
	{
		if (ImGui::Button(entity_name.c_str()))
		{
			Entity::ConstructParameters params;
			params.mWorld = gApp.GetWorld();
			params.mName = entity_name;
			Ref<Entity> entity = gEntityFactory.CreateClass(entity_name, params);
			entity->SetGUID(GUID::sCreate());
			gApp.GetWorld()->AddEntity(entity);
			fm::vec2 position = gRenderer.GetCamera()->GetPosition();
			entity->SetPosition(position);
		}
	}

	ImGui::End();
}
