#include "precomp.h"
#include "Engine/Entity/BaseEntity.h"

// Engine includes
#include "Engine/Factory/Factory.h"
#include "Engine/World/World.h"

Json BaseEntity::Serialize() const
{
	Json json;
	const Array<String>& component_names = gComponentFactory.GetClassNames();
	Json& json_component = json[GetClassName()]["Components"];
	for (const String& component_name : component_names)
	{
		if (gComponentFactory.HasComponent(component_name, GetRegistry(), GetEntityHandle()))
		{
			json_component[component_name] = gComponentFactory.GetComponent(component_name, GetRegistry(), GetEntityHandle())->Serialize();
		}
	}
	return json;
}

void BaseEntity::Deserialize(const Json& inJson)
{
	if (inJson.contains("Components"))
	{
		const Json& json_components = inJson["Components"];

		const Array<String>& component_names = gComponentFactory.GetClassNames();
		for (const String& component_name : component_names)
		{
			if (!json_components.contains(component_name))
				continue;

			ComponentBase* component;

			bool has_component = gComponentFactory.HasComponent(component_name, GetRegistry(), GetEntityHandle());
			if (has_component)
				component = gComponentFactory.GetComponent(component_name, GetRegistry(), GetEntityHandle());
			else
				component = gComponentFactory.CreateComponent(component_name, GetRegistry(), GetEntityHandle());

			component->Deserialize(json_components[component_name]);
		}
	}
}

BaseEntity::BaseEntity(World* inWorld)
{
	GenerateNewEntityHandle(inWorld);
}

BaseEntity::BaseEntity(entt::entity inHandle, World* inWorld)
{
	mWorld = inWorld;
	mEntityHandle = inHandle;
}

BaseEntity::~BaseEntity()
{
	if (mHandleWasGenerated)
		GetRegistry().destroy(mEntityHandle);
}

void BaseEntity::GenerateNewEntityHandle(World* inWorld)
{
	ScopedMutexWriteLock lock(inWorld->mEntitiesMutex);

	mWorld = inWorld;
	mHandleWasGenerated = true;
	entt::registry& registry = mWorld->GetRegistry();
	mEntityHandle = registry.create();
}

entt::registry& BaseEntity::GetRegistry() const
{
	return mWorld->GetRegistry();
}
