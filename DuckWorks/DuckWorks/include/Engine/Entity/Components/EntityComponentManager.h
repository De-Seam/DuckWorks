#pragma once
// Core includes
#include "Core/CoreBase.h"
#include "Core/Utilities/Mutex.h"
#include "Core/Allocators/ClassAllocator.h"

// Engine includes
#include "Engine/Threads/ThreadManager.h"

class Entity;
class EntityComponent;

class EntityComponentManager
{
public:

	template<typename taType>
	void LoopOverComponents(Function<void(taType& inComponent)> inFunction);

private:

};

extern EntityComponentManager gEntityComponentManager;

template<typename taType>
void EntityComponentManager::LoopOverComponents(Function<void(taType& inComponent)> inFunction)
{
	ClassAllocator<taType>& allocator = taType::sGetClassAllocator();
	const Array<void*>& pages = allocator.GetPages();
	uint64 page_size = allocator.GetPageSize();

	for (void* page : pages)
	{
		for (void* ptr = page; ptr < RCast<void*>((uint64)page + page_size); ptr = RCast<void*>((uint64)ptr + sizeof(bool) + sizeof(taType)))
		{
			// If the ptr is available and thus not valid, then we skip it
			if (*RCast<bool*>(ptr) == true)
				continue;

			taType* class_ptr = RCast<taType*>((uint64)ptr + sizeof(bool));
			inFunction(*class_ptr);
		}
	}
}
