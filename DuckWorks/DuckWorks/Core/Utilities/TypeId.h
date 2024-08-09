#pragma once
#include <Core/Containers/Atomic.h>
#include <Core/Utilities/Types.h>

#define STATIC_TYPE_ID(inName) \
public: \
	static const inName##TypeID& sGet##inName##TypeID() { return s##inName##TypeID; }; \
	virtual const inName##TypeID& Get##inName##TypeID() const { return s##inName##TypeID; } \
\
private: \
	inline static inName##TypeID s##inName##TypeID = {}; \
public:

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

namespace std
{
	template <typename T> struct hash;

	template<typename taType>
	struct hash<TypeID<taType>>
	{
		size_t operator()(const TypeID<taType>& inTypeID) const
		{
			return static_cast<size_t>(inTypeID);
		}
	};

}
