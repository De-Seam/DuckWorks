#pragma once
#include "Types.h"

class UID
{
public:
	UID();
	UID(const UID& inOther) = default;

	operator uint64() const { return mUID; }
private:
	uint64 mUID;
};

#include <vcruntime.h>

namespace std
{
	template <typename T> struct hash;

	template<>
	struct hash<UID>
	{
		size_t operator()(const UID& inUID) const
		{
			return inUID;
		}
	};

}