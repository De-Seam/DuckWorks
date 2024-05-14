#pragma once
// Core includes
#include "Core/CoreBase.h"
#include "Core/Utilities/Mutex.h"

// Engine includes
#include "Engine/Threads/ThreadManager.h"

class Entity;
class EntityComponent;

class EntityComponentManager
{
public:
	template<typename taType>
	void RegisterComponentType();

	template<typename taType, typename... taArgs>
	Handle<EntityComponent> AddComponent(taArgs&&... inArgs);
	template<typename taType>
	MutexReadProtectedPtr<taType> GetComponent(Handle<EntityComponent> inEntityComponentHandle);
	MutexReadProtectedPtr<EntityComponent> GetComponent(Handle<EntityComponent> inEntityComponentHandle, UID inComponentTypeUID);
	template<typename taType>
	void RemoveComponent(Handle<EntityComponent> inEntityComponentHandle);

	template<typename taType>
	void LoopOverComponents(Function<void(taType& inComponent)> inFunction);

private:
	template<typename taType, typename... taArgs>
	struct EntityComponentData
	{
		EntityComponentData(taArgs&&... inArgs) :
			mComponent(std::forward<taArgs>(inArgs)...) {}

		bool mAlive = true;
		taType mComponent;
	};

	struct EntityComponentLine
	{
		void* mComponents = nullptr; // Array of EntityComponentDatas, it's Array<Pair<EntityComponentData>> with the bool flag to determine whether it's alive
		Array<uint64> mFreeIndices; // Indices of free slots in the array
		UniquePtr<Mutex> mMutex; // One mutex per EntityComponent type
	};

	// Maps [EntityComponentTypeUID] -> [EntityComponent Array]
	HashMap<UID, EntityComponentLine> mEntityComponentLinesMap;
	Mutex mEntityComponentsDataMapMutex;
};

extern EntityComponentManager gEntityComponentManager;

template<typename taType>
void EntityComponentManager::RegisterComponentType()
{
	gAssert(gGetCurrentThreadIndex() == 0, "Registering should happen on the main thread!");
	ScopedMutexWriteLock lock(mEntityComponentsDataMapMutex);

	EntityComponentLine& line = mEntityComponentLinesMap[taType::sGetRTTIUID()];
	line.mComponents = new Array<EntityComponentData<taType>>();
	line.mMutex = gMakeUnique<Mutex>();
}

template<typename taType, typename... taArgs>
Handle<EntityComponent> EntityComponentManager::AddComponent(taArgs&&... inArgs)
{
	EntityComponentLine& entity_component_line = mEntityComponentLinesMap[taType::sGetRTTIUID()];
	ScopedMutexWriteLock lock(*entity_component_line.mMutex);
	Array<EntityComponentData<taType>>* component_datas = RCast<Array<EntityComponentData<taType>>*>(entity_component_line.mComponents);

	if (!entity_component_line.mFreeIndices.empty())
	{
		uint64 free_index = entity_component_line.mFreeIndices.back();
		entity_component_line.mFreeIndices.pop_back();
		(*component_datas)[free_index] = EntityComponentData<taType>(std::forward<taArgs>(inArgs)...);
		return Handle<EntityComponent>(free_index, {});
	}
	component_datas->emplace_back(EntityComponentData<taType>(std::forward<taArgs>(inArgs)...));
	return Handle<EntityComponent>(component_datas->size() - 1);
}

template<typename taType>
MutexReadProtectedPtr<taType> EntityComponentManager::GetComponent(Handle<EntityComponent> inEntityComponentHandle)
{
	EntityComponentLine& entity_component_line = mEntityComponentLinesMap[taType::sGetRTTIUID()];
	Array<EntityComponentData<taType>>* component_datas = RCast<Array<EntityComponentData<taType>>*>(entity_component_line.mComponents);
	return MutexReadProtectedPtr<taType>(*entity_component_line.mMutex, &(*component_datas)[inEntityComponentHandle.mIndex].mComponent);
}

template<typename taType>
void EntityComponentManager::RemoveComponent(Handle<EntityComponent> inEntityComponentHandle)
{
	EntityComponentLine& entity_component_line = mEntityComponentLinesMap[taType::sGetRTTIUID()];
	ScopedMutexWriteLock lock(*entity_component_line.mMutex);
	Array<EntityComponentData<taType>>& component_datas = *RCast<Array<EntityComponentData<taType>*>>(entity_component_line.mComponents);
	component_datas[inEntityComponentHandle.mIndex].mComponent.operator~();
	entity_component_line.mFreeIndices.push_back(inEntityComponentHandle.mIndex);
}

template<typename taType>
void EntityComponentManager::LoopOverComponents(Function<void(taType& inComponent)> inFunction)
{
	EntityComponentLine& entity_component_line = mEntityComponentLinesMap[taType::sGetRTTIUID()];
	ScopedMutexReadLock lock(*entity_component_line.mMutex);
	Array<EntityComponentData<taType>>& component_datas = *RCast<Array<EntityComponentData<taType>*>>(entity_component_line.mComponents);
	for (EntityComponentData<taType>& component_data : component_datas)
	{
		if (component_data.mAlive)
			inFunction(component_data.mComponent);
	}
}
