#pragma once
// CoreBase includes the core utilities and math
#include "Core/Utilities/UID.h"
#include "RTTI/RefObject.h"
#include "Utilities/Types.h"
#include "Utilities/Utilities.h"

// General Handle pointing to an index. The default constructor makes an invalid handle.
template<typename taType>
struct Handle
{
	Handle() = default;

	Handle(uint64 inIndex, UID inUID)
		: mIndex(inIndex)
		, mUID(inUID) {}

	Handle(uint64 inIndex)
		: mIndex(inIndex) {}

	uint64 mIndex = UINT64_MAX;
	UID mUID = {};

	bool operator==(const Handle& inObjectHandle) const
	{
		bool result = mIndex == inObjectHandle.mIndex;
#ifdef _DEBUG
		if (result)
		{
			gAssert(mUID == inObjectHandle.mUID, "Invalid Handle!");
		}
#endif
		return result;
	}

	bool IsValid() const
	{
		return mIndex != UINT64_MAX;
	}

	Handle& operator=(const Handle& inOther)
	{
		//gAssert(mIndex == UINT64_MAX, "Old handle was destroyed!");
		mIndex = inOther.mIndex;
		mUID = inOther.mUID;
		return *this;
	}

	Handle(const Handle& inOther)
	{
		mIndex = inOther.mIndex;
		mUID = inOther.mUID;
	}
};
