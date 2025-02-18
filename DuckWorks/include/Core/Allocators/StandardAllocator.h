#pragma once
#include "AllocatorBase.h"

template<typename taType>
class StandardAllocator : public AllocatorBase
{
public:
	taType* Allocate(IF_TRACK_ALLOCATIONS(const DC::String& inAllocationOrigin));

	void Free(taType* inPtr);
private:

};

template<typename taType>
inline taType* StandardAllocator<taType>::Allocate(IF_TRACK_ALLOCATIONS(const DC::String& inAllocationOrigin))
{
	taType* return_ptr = RCast<taType*>(malloc(sizeof(taType)));
	IF_TRACK_ALLOCATIONS(TrackAllocation(inAllocationOrigin, return_ptr));
	return return_ptr;
}

template<typename taType>
inline void StandardAllocator<taType>::Free(taType* inPtr)
{
	IF_TRACK_ALLOCATIONS(UntrackAllocation(inPtr));
	free(inPtr);
}
