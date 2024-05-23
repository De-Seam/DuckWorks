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
	if (mAllocations.erase(inAllocatedPtr) == 0)
		gAssert(false, "Trying to untrack allocation that was not tracked!");
}

#endif // TRACK_ALLOCATIONS
