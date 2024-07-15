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
	AllocatorBase();
	virtual ~AllocatorBase();

	virtual String GetName() const = 0;

	template<typename taType, typename... taArgs>
#ifdef TRACK_ALLOCATIONS
	taType* Allocate(const String& inAllocationOrigin, taArgs&&... inArgs);

	struct AllocationData
	{
		String mOrigin;
		uint64 mSize;
	};

	HashMap<void*, AllocationData>& GetAllocations() { return mAllocations; }
#else
	taType *Allocate(taArgs &&...inArgs);
#endif // TRACK_ALLOCATIONS

protected:
#ifdef TRACK_ALLOCATIONS
	void TrackAllocation(void* inAllocatedPtr, const String& inAllocationOrigin, uint64 inSize);
	void UntrackAllocation(void* inAllocatedPtr);

	HashMap<void*, AllocationData> mAllocations;
#endif // TRACK_ALLOCATIONS
};

#ifdef TRACK_ALLOCATIONS

// Array with all allocator
extern Array<AllocatorBase*>* gAllocators;

#endif // TRACK_ALLOCATIONS

template<typename taType, typename... taArgs>
#ifdef TRACK_ALLOCATIONS
taType* AllocatorBase::Allocate(const String& inAllocationOrigin, taArgs&&... inArgs)
#else
taType* AllocatorBase::Allocate(taArgs &&...inArgs)
#endif // TRACK_ALLOCATIONS
{
	IF_TRACK_ALLOCATIONS((void)inAllocationOrigin;)
	(void)inArgs;
	gAssert(false, "AllocatorBase should not be used!");
	return nullptr;
}
