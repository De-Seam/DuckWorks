#include "Precomp.h"
#include "Engine/Entity/Components/EntityComponentManager.h"

// Engine includes
#include "Engine/Entity/Components/EntityComponent.h"

EntityComponentManager gEntityComponentManager;

MutexReadProtectedPtr<EntityComponent> EntityComponentManager::GetComponent(Handle<EntityComponent> inEntityComponentHandle, UID inComponentTypeUID)
{
	EntityComponentLine& entity_component_line = mEntityComponentLinesMap[inComponentTypeUID];
	EntityComponent* component = entity_component_line.mGetFunction(entity_component_line, inEntityComponentHandle);

	return MutexReadProtectedPtr<EntityComponent>(*entity_component_line.mMutex, component);
}

void EntityComponentManager::SetEntityOnEntityComponent(Entity* inEntity, EntityComponent* inEntityComponent)
{
	inEntityComponent->mEntity = inEntity;
}
