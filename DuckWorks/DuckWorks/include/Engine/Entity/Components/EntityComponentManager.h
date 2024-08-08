#pragma once
// Core includes
#include "Core/CoreBase.h"
#include "Core/Allocators/ClassAllocator.h"

class EntityComponentManager : public Manager
{
	RTTI_CLASS(EntityComponentManager, Manager)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	EntityComponentManager(const ConstructParameters& inParameters = {}) : Base(inParameters) {}

	template<typename taType>
	void LoopOverComponents(Function<void(taType& inComponent)> inFunction);

	template<typename taType>
	void UpdateComponents(float inDeltaTime);
};

extern EntityComponentManager gEntityComponentManager;

template<typename taType>
void EntityComponentManager::LoopOverComponents(Function<void(taType& inComponent)> inFunction)
{
	ClassAllocator<taType>& allocator = taType::sGetAllocator();
	const Array<void*>& pages = allocator.GetPages();
	uint64 page_size = allocator.GetPageSize();

	for (void* page : pages)
	{
		for (void* ptr = page; ptr < RCast<void*>((uint64)page + page_size); ptr = RCast<void*>((uint64)ptr + sizeof(bool) + sizeof(taType)))
		{
			const uint8& is_available = *RCast<uint8*>(ptr);
			// If the ptr is available and thus not valid, then we skip it
			if (is_available == 1)
				continue;

			taType* class_ptr = RCast<taType*>((uint64)ptr + sizeof(bool));
			inFunction(*class_ptr);
		}
	}
}

template<typename taType>
void EntityComponentManager::UpdateComponents(float inDeltaTime)
{
	LoopOverComponents<taType>([inDeltaTime](taType& inComponent)
	{
		inComponent.Update(inDeltaTime);
	});
}
