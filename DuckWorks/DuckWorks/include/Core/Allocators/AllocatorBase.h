#pragma once
#include "Core/Utilities/Types.h"
#include "Core/Utilities/Utilities.h"

#ifdef _DEBUG
#define TRACK_ALLOCATIONS
#define IF_TRACK_ALLOCATIONS(x) x
#define ALLOC_TRACK __FILE__ ":" + std::to_string(__LINE__)
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
	taType* Allocate(IF_TRACK_ALLOCATIONS(const String& inAllocationOrigin), taArgs&&... inArgs);

protected:
#ifdef TRACK_ALLOCATIONS
	void TrackAllocation(const String& inAllocationOrigin, void* inAllocatedPtr);
	void UntrackAllocation(void* inAllocatedPtr);

	HashMap<void*, String> mAllocations;
#endif // TRACK_ALLOCATIONS
};

template<typename taType, typename... taArgs>
taType* AllocatorBase::Allocate(IF_TRACK_ALLOCATIONS(const String& inAllocationOrigin), taArgs&&... inArgs)
{
	IF_TRACK_ALLOCATIONS((void)inAllocationOrigin;)
	(void)inArgs;
	gAssert(false, "AllocatorBase should not be used!");
	return nullptr;
}
