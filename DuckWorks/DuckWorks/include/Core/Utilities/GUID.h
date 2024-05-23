#pragma once
#include "Types.h"

class GUID
{
public:
	GUID() = default;
	GUID(const String& inGUIDString);
	GUID(const GUID& inOther) = default;

	uint64 GetValue() const { return mGUID; }
	operator uint64() const { return mGUID; }

	static GUID sCreate();

	String ToString() const;
	friend size_t hash_value(const GUID &inGUID) { return inGUID.mGUID; }

private:
	uint64 mGUID = 0;
};

#include <vcruntime.h>

namespace std
{
	template <typename T> struct hash;

	template<>
	struct hash<GUID>
	{
		size_t operator()(const GUID& inUID) const
		{
			return inUID;
		}
	};

}