#pragma once
#include "AllocatorBase.h"
#include "Core/Utilities/Mutex.h"

// Allocator to allocate instances of a specific class
template<typename taType>
class ClassAllocator : public AllocatorBase
{
public:
	ClassAllocator(uint64 inClassAmount = 64, uint64 inInitialPages = 1);
	virtual ~ClassAllocator() override;

	taType* Allocate(IF_TRACK_ALLOCATIONS(const String& inAllocationOrigin));
	void Free(taType* inPtr);

	const Array<void*>& GetPages() const { return mPages; }
	uint64 GetPageSize() const { return mPageSize; }

protected:
	void* CreateNewPage();

	// A Class Allocator can have an unlimited amount of "pages", which are simply bulks of data
	// Each page is [bool : Is Available][taType]
	Array<void*> mPages;
	uint64 mPageSize;

	UniqueMutex mMutex;
};

template<typename taType>
ClassAllocator<taType>::ClassAllocator(uint64 inClassAmount, uint64 inInitialPages)
	: mPageSize(inClassAmount * (sizeof(bool) + sizeof(taType)))
{
	for (uint64 i = 0; i < inInitialPages; i++)
		CreateNewPage();
}

template<typename taType>
ClassAllocator<taType>::~ClassAllocator()
{
	mPages.clear();
}

template<typename taType>
taType* ClassAllocator<taType>::Allocate(IF_TRACK_ALLOCATIONS(const String& inAllocationOrigin))
{
	ScopedUniqueMutexLock lock(mMutex);
	for (void* page : mPages)
	{
		for (void* ptr = page; ptr < RCast<void*>((uint64)page + mPageSize); ptr = RCast<void*>((uint64)ptr + sizeof(bool) + sizeof(taType)))
		{
			if (*SCast<bool*>(ptr) == true)
			{
				*SCast<bool*>(ptr) = false;
				taType* return_ptr = RCast<taType*>((uint64)ptr + sizeof(bool));
				IF_TRACK_ALLOCATIONS(TrackAllocation(return_ptr, inAllocationOrigin, sizeof(taType)));
				return return_ptr;
			}
		}
	}
	CreateNewPage();
	void* page = mPages.back();
	void* ptr = page;
	*SCast<bool*>(ptr) = false;
	taType* return_ptr = RCast<taType*>((uint64)ptr + sizeof(bool));
	IF_TRACK_ALLOCATIONS(TrackAllocation(return_ptr, inAllocationOrigin, sizeof(taType)));
	return return_ptr;
}

template<typename taType>
void ClassAllocator<taType>::Free(taType* inPtr)
{
	ScopedUniqueMutexLock lock(mMutex);

	IF_TRACK_ALLOCATIONS(UntrackAllocation(inPtr));

	bool* is_available_ptr = RCast<bool*>((uint64)inPtr - sizeof(bool));
	bool& is_available_bool = *SCast<bool*>(is_available_ptr);
	gAssert(is_available_bool == false, "Free called on Memory that was not Allocated!");
	is_available_bool = true;
}

template<typename taType>
void* ClassAllocator<taType>::CreateNewPage()
{
	void* page = malloc(mPageSize);
	gAssert(page != nullptr, "Failed to allocate memory for new page");
	// memset to 1 to set the Available flags to true
	memset(page, 0x01010101, mPageSize);
	mPages.emplace_back(page);
	return page;
}
