#include "Precomp.h"
#include "Engine/Entity/Components/EntityComponentManager.h"

#include "Engine/Entity/Components/EntityComponent.h"

EntityComponentManager gEntityComponentManager;

MutexReadProtectedPtr<EntityComponent> EntityComponentManager::GetComponent(Handle<EntityComponent> inEntityComponentHandle, UID inComponentTypeUID)
{
	EntityComponentLine& entity_component_line = mEntityComponentLinesMap[inComponentTypeUID];
	Array<EntityComponentData<EntityComponent>>* component_datas = RCast<Array<EntityComponentData<EntityComponent>>*>(entity_component_line.mComponents);
	return MutexReadProtectedPtr<EntityComponent>(*entity_component_line.mMutex, &(*component_datas)[inEntityComponentHandle.mIndex].mComponent);
}

void EntityComponentManager::SetEntityOnEntityComponent(Entity* inEntity, EntityComponent* inEntityComponent)
{
	inEntityComponent->mEntity = inEntity;
}
