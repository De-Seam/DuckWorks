#pragma once
#include <Core/Utilities/Types.h>
#include <Core/Containers/Atomic.h>

// TypeID increments by 1 each time it's instantiated
// taType for the base type
template<typename taType>
class TypeID
{
public:
	TypeID();
	TypeID(const TypeID& inOther) : mID(inOther.mID) {}

	int32 Get() const { return mID; }
	operator int32() const { return mID; }

private:
	int32 mID = -1;
};

template<typename taType>
inline TypeID<taType>::TypeID()
{
	static Atomic<int32> sNextID = 0;

	mID = sNextID++;
}
