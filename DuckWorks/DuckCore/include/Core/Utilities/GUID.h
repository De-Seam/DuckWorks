#pragma once
// Core includes
#include <Core/Containers/String.h>
#include <Core/Utilities/Json.h>
#include <Core/Utilities/Types.h>

class GUID
{
public:
	GUID() = default;
	GUID(const String& inGUIDString);
	GUID(const char* inChars);
	GUID(const GUID& inOther) = default;

	uint64 GetValue() const { return mGUID; }
	operator uint64() const { return mGUID; }

	static GUID sCreate();
	static GUID sCombine(const GUID& inGUID1, const GUID& inGUID2, int32 inSalt = 0);

	String ToString() const;
	friend size_t hash_value(const GUID& inGUID) { return inGUID.mGUID; }

private:
	uint64 mGUID = 0;
};

void to_json(Json& outJson, const GUID& inVariable);
void from_json(const Json& inJson, GUID& outVariable);

namespace std
{
template<typename T>
struct hash;

template<>
struct hash<GUID>
{
	size_t operator()(const GUID& inUID) const
	{
		return inUID;
	}
};
}