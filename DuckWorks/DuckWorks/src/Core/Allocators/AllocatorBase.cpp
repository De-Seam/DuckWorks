#include "Precomp.h"
#include "Core/Allocators/AllocatorBase.h"

#ifdef TRACK_ALLOCATIONS

Array<AllocatorBase*>* gAllocators = nullptr;

#endif // TRACK_ALLOCATIONS

AllocatorBase::AllocatorBase()
{
#ifdef TRACK_ALLOCATIONS

	if (!gAllocators)
		gAllocators = new Array<AllocatorBase*>();

	gAllocators->push_back(this);

#endif
}

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

void AllocatorBase::TrackAllocation(void* inAllocatedPtr, const String& inAllocationOrigin, uint64 inSize)
{
	AllocationData data;
	data.mOrigin = inAllocationOrigin;
	data.mSize = inSize;
	mAllocations[inAllocatedPtr] = data;
}

void AllocatorBase::UntrackAllocation(void* inAllocatedPtr)
{
	if (mAllocations.erase(inAllocatedPtr) == 0)
		gAssert(false, "Trying to untrack allocation that was not tracked!");
}

#endif // TRACK_ALLOCATIONS
