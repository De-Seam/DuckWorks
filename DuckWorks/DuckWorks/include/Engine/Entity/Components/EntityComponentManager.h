#pragma once
// Core includes
#include "Core/CoreBase.h"
#include "Core/Utilities/Mutex.h"

// Engine includes
#include "Engine/Threads/ThreadManager.h"

class Entity;
class EntityRefComponent;

class EntityComponentManager
{
public:
	template<typename taType>
	void RegisterComponentType();

	template<typename taType>
	Handle<EntityRefComponent> AddComponent();
	template<typename taType>
	MutexReadProtectedPtr<taType> GetComponent(Handle<EntityRefComponent> inEntityComponentHandle);

private:
	template<typename taType>
	struct EntityComponentData
	{
		bool mAlive = true;
		taType mComponent;
	};

	struct EntityComponentLine
	{
		void* mComponents = nullptr; // Array of EntityComponents, it's Array<Pair<bool, taType>> with the bool flag to determine whether it's alive
		Array<uint64> mFreeIndices; // Indices of free slots in the array
		Mutex mMutex; // One mutex per EntityComponent type
	};

	// Maps [EntityComponentTypeUID] -> [EntityComponent Array]
	HashMap<UID, EntityComponentLine> mEntityComponentLinesMap;
	Mutex mEntityComponentsDataMapMutex;
};

extern EntityComponentManager gEntityComponentManager;

template<typename taType>
inline void EntityComponentManager::RegisterComponentType()
{
	gAssert(gGetCurrentThreadIndex() == 0, "Registering should happen on the main thread!");
	ScopedMutexWriteLock lock(mEntityComponentsDataMapMutex);

	EntityComponentLine& line = mEntityComponentLinesMap[taType::sGetRTTIUID()];
	line.mComponents = new Array<EntityComponentData<taType>>();

}

template<typename taType>
inline Handle<EntityRefComponent> EntityComponentManager::AddComponent()
{
	EntityComponentLine& entity_component_line = mEntityComponentLinesMap[taType::sGetRTTIUID()];
	ScopedMutexWriteLock lock(entity_component_line.mMutex);
	Array<EntityComponentData<taType>>* component_datas = RCast<Array<EntityComponentData<taType>*>>(entity_component_line.mComponents);
	
	if (!entity_component_line.mFreeIndices.empty())
	{
		uint64 free_index = entity_component_line.mFreeIndices.back();
		entity_component_line.mFreeIndices.pop_back();
		*component_datas[free_index] = EntityComponentData<taType>();
		return Handle<EntityRefComponent>(free_index);
	}
	component_datas->emplace_back(EntityComponentData<taType>());
	return Handle<EntityRefComponent>(component_datas->size() - 1);
}

template<typename taType>
inline MutexReadProtectedPtr<taType> EntityComponentManager::GetComponent(Handle<EntityRefComponent> inEntityComponentHandle)
{
	EntityComponentLine& entity_component_line = mEntityComponentLinesMap[taType::sGetRTTIUID()];
	Array<EntityComponentData<taType>>* component_datas = RCast<Array<EntityComponentData<taType>*>>(entity_component_line.mComponents);
	return MutexReadProtectedPtr<taType>(entity_component_line.mMutex, *component_datas[inEntityComponentHandle.mIndex]);
}
