#include "Precomp.h"
#include "Core/Allocators/AllocatorBase.h"


AllocatorBase::~AllocatorBase()
{
#ifdef TRACK_ALLOCATIONS

	if (!mAllocations.empty())
	{
		gLog(ELogType::Error, "Allocator destroyed before all its memory was freed!");
		gAssert(false, "Allocator destroyed before all its memory was freed!");
	}

#endif // TRACK_ALLOCATIONS
}

#ifdef TRACK_ALLOCATIONS

void AllocatorBase::TrackAllocation(const String& inAllocationOrigin, void* inAllocatedPtr)
{
	mAllocations[inAllocatedPtr] = inAllocationOrigin;
}

void AllocatorBase::UntrackAllocation(void* inAllocatedPtr)
{
	mAllocations.erase(inAllocatedPtr);
}

#endif // TRACK_ALLOCATIONS