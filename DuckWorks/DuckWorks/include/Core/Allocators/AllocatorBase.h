#pragma once
#include "Core/CoreBase.h"

#ifdef _DEBUG
#define TRACK_ALLOCATIONS
#define IF_TRACK_ALLOCATIONS(x) x
#define ALLOC_TRACK __FILE__
#else
#define IF_TRACK_ALLOCATIONS(x)
#define ALLOC_TRACK 
#endif

#define gNew

class AllocatorBase
{
public:
	virtual ~AllocatorBase();

	template<typename taType, typename... taArgs>
	taType* New(IF_TRACK_ALLOCATIONS(const String& inAllocationOrigin), taArgs&&... inArgs);

protected:
#ifdef TRACK_ALLOCATIONS
	void TrackAllocation(const String& inAllocationOrigin, void* inAllocatedPtr);
	void UntrackAllocation(void* inAllocatedPtr);

	HashMap<void*, String> mAllocations;
#endif // TRACK_ALLOCATIONS
};

template<typename taType, typename... taArgs>
inline taType* AllocatorBase::New(IF_TRACK_ALLOCATIONS(const String& inAllocationOrigin), taArgs &&... inArgs)
{
	IF_TRACK_ALLOCATIONS((void)inAllocationOrigin;)
	(void)inArgs;
	gAssert(false, "AllocatorBase should not be used!");
	return nullptr;
}