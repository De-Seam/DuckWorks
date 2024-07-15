#pragma once
#include "AllocatorBase.h"

template<typename taType>
class StandardAllocator : public AllocatorBase
{
public:
	taType* Allocate(IF_TRACK_ALLOCATIONS(const String& inAllocationOrigin));

	void Free(taType* inPtr);

private:
};

template<typename taType>
taType* StandardAllocator<taType>::Allocate(IF_TRACK_ALLOCATIONS(const String& inAllocationOrigin))
{
	taType* return_ptr = RCast<taType*>(malloc(sizeof(taType)));
	IF_TRACK_ALLOCATIONS(TrackAllocation(return_ptr, inAllocationOrigin, sizeof(taType)));
	return return_ptr;
}

template<typename taType>
void StandardAllocator<taType>::Free(taType* inPtr)
{
	IF_TRACK_ALLOCATIONS(UntrackAllocation(inPtr));
	free(inPtr);
}
