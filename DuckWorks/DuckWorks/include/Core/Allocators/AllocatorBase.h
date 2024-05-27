#pragma once
#include "Core/Utilities/Types.h"
#include "Core/Utilities/Utilities.h"

#ifdef _DEBUG
#define TRACK_ALLOCATIONS
#endif //_DEBUG

#ifdef TRACK_ALLOCATIONS
#define IF_TRACK_ALLOCATIONS(x) x
#define ALLOC_TRACK __FILE__ ":" + std::to_string(__LINE__)
#else
#define IF_TRACK_ALLOCATIONS(x) 
#define ALLOC_TRACK 
#endif // TRACK_ALLOCATIONSS

class AllocatorBase
{
public:
	virtual ~AllocatorBase();

	template<typename taType, typename... taArgs>
#ifdef TRACK_ALLOCATIONS
	taType* Allocate(const String& inAllocationOrigin, taArgs&&... inArgs);
#else
	taType *Allocate(taArgs &&...inArgs);
#endif // TRACK_ALLOCATIONS

protected:
#ifdef TRACK_ALLOCATIONS
	void TrackAllocation(const String& inAllocationOrigin, void* inAllocatedPtr);
	void UntrackAllocation(void* inAllocatedPtr);

	HashMap<void*, String> mAllocations;
#endif // TRACK_ALLOCATIONS
};

template<typename taType, typename... taArgs>
#ifdef TRACK_ALLOCATIONS
taType* Allocate(const String& inAllocationOrigin, taArgs&&... inArgs)
#else
taType* Allocate(taArgs &&...inArgs)
#endif // TRACK_ALLOCATIONS
{
	IF_TRACK_ALLOCATIONS((void)inAllocationOrigin;)
	(void)inArgs;
	gAssert(false, "AllocatorBase should not be used!");
	return nullptr;
}
