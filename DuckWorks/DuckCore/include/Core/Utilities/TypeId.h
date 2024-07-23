#pragma once
#include <Core/Utilities/Types.h>

// taType for the base type
template<typename taType>
class TypeId
{
public:
	TypeId() = default;
	~TypeId() = default;
	TypeId(int32 inId) : mId(inId) {}
	TypeId(const TypeId& inOther) : mId(inOther.mId) {}

	int32 Get() const { return mId; }
	operator int32() const { return mId; }

private:
	int32 mId = -1;
};
