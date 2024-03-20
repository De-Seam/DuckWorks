#pragma once
// CoreBase includes the core utilities and math
#include "Utilities/Types.h"
#include "Utilities/Utilities.h"
#include "Core/Utilities/UID.h"

// General Handle pointing to an index. The default constructor makes an invalid handle.
template<typename taType>
struct Handle
{
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
};
