#pragma once
// Core includes
#include <Core/Containers/Pair.h>
#include <Core/Utilities/Json.h>
#include <Core/Utilities/Types.h>

// Std includes
#include <string>

class String
{
public:
	String() = default;
	String(const char* inCharacters);
	String(const std::string& inString);
	String(const String& inOther);
	~String();

	static String sFromStdString(std::string inString);

	void Clear();
	uint32 Length() const { return mLength; }

	const char* operator*() const { return mCharacters; }

	String& operator=(const String& inOther);
	bool operator==(const String& inOther) const;
	bool operator==(const char* inCharacters) const;

	String operator+(const String& inOther) const;
	String& operator+=(const String& inOther);

	uint64 GetHash() const { return mHash; }

	bool Contains(const char* inCharacters) const;
	void Replace(int32 inStart, int32 inEnd, const char* inValue);

private:
	void SetCharacters(const char* inCharacters, uint32 inLength);

	char* mCharacters = nullptr;
	uint32 mLength = 0;
	uint64 mHash = 0;
};

void to_json(Json& outJson, const String& inVariable);
void from_json(const Json& inJson, String& outVariable);

uint64 gHash(const char* inCharacters, uint32 inLength, uint64 inSeed = 0);
Pair<uint64, uint64> gHash_128(const char* inCharacters, uint32 inLength, uint64 inSeed = 0);

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