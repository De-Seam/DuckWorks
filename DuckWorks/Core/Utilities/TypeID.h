#pragma once
#include <Core/Config.h>
#include <Core/Containers/Atomic.h>
#include <Core/Containers/String.h>
#include <Core/Utilities/Types.h>

#define STATIC_TYPE_ID(inName) \
public: \
	static const inName##TypeID& sGet##inName##TypeID() { return s##inName##TypeID; }; \
	virtual const inName##TypeID& Get##inName##TypeID() const { return s##inName##TypeID; } \
\
private: \
	inline static inName##TypeID s##inName##TypeID = {}; \
public:

#define DECLARE_TYPE_ID(inTypeIDName) \
class inTypeIDName : public TypeID \
{ \
public: \
	inTypeIDName() : TypeID(gGetAndIncrementTypeID(#inTypeIDName)) {} \
}; \
\
namespace std \
{ \
	template<typename taType> struct hash; \
\
	template<> \
	struct hash<inTypeIDName> \
	{ \
		size_t operator()(const inTypeIDName& inTypeID) const \
		{ \
			return static_cast<size_t>(inTypeID); \
		} \
	}; \
}

// TypeID increments by 1 each time it's instantiated
// taType for the base type
class TypeID
{
public:
	TypeID(const TypeID& inOther) : mID(inOther.mID) {}

	int32 Get() const { return mID; }
	operator int32() const { return mID; }

protected:
	TypeID(int32 inID) : mID(inID) {}

private:
	int32 mID = -1;
};

int32 CORE_API gGetAndIncrementTypeID(const String& inTypeIDName);

namespace std
{
	template<typename taType> struct hash;

	template<>
	struct hash<TypeID>
	{
		size_t operator()(const TypeID& inTypeID) const
		{
			return static_cast<size_t>(inTypeID);
		}
	};
}
