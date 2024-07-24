#pragma once
// Core includes
#include <Core/Utilities/Types.h>

class String
{
public:
	String() = default;
	String(const char* inCharacters);
	String(const String& inOther);
	~String();

	void Clear();
	uint32 Length() const { return mLength; }

	const char* operator*() const { return mCharacters; }

	String& operator=(const String& inOther);
	bool operator==(const String& inOther) const;
	bool operator==(const char* inCharacters) const;
	bool operator!=(const String& inOther) const { return !(*this == inOther); }
	bool operator!=(const char* inCharacters) const { return !(*this == inCharacters); }

	String operator+(const String& inOther) const;
	String& operator+=(const String& inOther);

	uint64 GetHash() const { return mHash; }

	bool Contains(const char* inCharacters) const;

private:
	void SetCharacters(const char* inCharacters, uint32 inLength);

	char* mCharacters = nullptr;
	uint32 mLength = 0;
	uint64 mHash = 0;
};

uint64 gHash(const char* inCharacters, uint32 inLength, uint64 inSeed = 0);

#include <vcruntime.h>

namespace std
{
	template<typename T>
	struct hash;

	template<>
	struct hash<String>
	{
		size_t operator()(const String& inString) const
		{
			return inString.GetHash();
		}
	};
}