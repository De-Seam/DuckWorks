#pragma once
#include "AllocatorBase.h"

// Allocator to alloce instances of a specific class
template<typename taType>
class ClassAllocator : public AllocatorBase
{
public:
	ClassAllocator(uint64 inClassAmount = 32, uint64 inInitialPages = 1);
	virtual ~ClassAllocator() override;

	taType* Allocate(IF_TRACK_ALLOCATIONS(const String& inAllocationOrigin));
	void Free(taType* inPtr);

protected:
	virtual void* CreateNewPage();

	// A Class Allocator can have an unlimited amount of "pages", which are simply bulks of data
	// Each page is [bool : Is Available][taType]
	Array<void*> mPages;
	uint64 mPageSize;
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
	for (void* page : mPages)
		free(page);
	mPages.clear();
}

template<typename taType>
inline taType* ClassAllocator<taType>::Allocate(IF_TRACK_ALLOCATIONS(const String& inAllocationOrigin))
{
	for (void* page : mPages)
	{
		for (void* ptr = page; ptr < page + mPageSize; ptr += sizeof(bool) + sizeof(taType))
		{
			if (*RCast<bool*>(ptr) == true)
			{
				*RCast<bool*>(ptr) = false;
				IF_TRACK_ALLOCATIONS(TrackAllocation(inAllocationOrigin, ptr + sizeof(bool)));
				return ptr + sizeof(bool);
			}
		}
	}
	CreateNewPage();
	void* page = mPages.back();
	void* ptr = page;
	*RCast<bool*>(ptr) = false;
	IF_TRACK_ALLOCATIONS(TrackAllocation(inAllocationOrigin, ptr + sizeof(bool)));
	return ptr + sizeof(bool);
}

template<typename taType>
void ClassAllocator<taType>::Free(taType* inPtr)
{
	void* ptr = inPtr - sizeof(bool);
	*RCast<bool*>(ptr) = true;
}

template<typename taType>
void* ClassAllocator<taType>::CreateNewPage()
{
	void* page = malloc(mPageSize);
	// memset to 1 to set the Available flags to true
	memset(page, 0x01010101, mPageSize);
	mPages.emplace_back(page);
	return page;
}