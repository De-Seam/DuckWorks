#include <Core/Precomp.h>
#include <Core/Containers/String.h>

// Core includes
#include <Core/Utilities/Assert.h>

// Std includes
#include <cstring>


String::String(const char* inCharacters)
{
	SetCharacters(inCharacters, static_cast<uint32>(strlen(inCharacters)));
}

String::String(const std::string& inString) 
{
	SetCharacters(inString.c_str(), static_cast<uint32>(inString.length()));
}

String::String(const String& inOther)
{
	SetCharacters(inOther.mCharacters, inOther.mLength);
}

String::~String()
{
	Clear();
}

String String::sFromStdString(std::string inString) 
{
	return String(inString.c_str());
}

void String::Clear()
{
	if (mCharacters != nullptr)
	{
		delete[] mCharacters;
		mCharacters = nullptr;
	}
	mLength = 0;
	mHash = 0;
}

String& String::operator=(const String& inOther)
{
	gAssert(this != &inOther);

	Clear();

	SetCharacters(inOther.mCharacters, inOther.mLength);

	return *this;
}

bool String::operator==(const String& inOther) const
{
	bool return_value = mHash == inOther.mHash;
#ifdef _DEBUG
	if (return_value)
		gAssert(strcmp(mCharacters, inOther.mCharacters) == 0);
#endif // _DEBUG
	return mHash == inOther.mHash;
}

bool String::operator==(const char* inCharacters) const
{
	return strcmp(mCharacters, inCharacters) == 0;
}

String String::operator+(const String& inOther) const
{
	String new_string;
	new_string.mLength = mLength + inOther.mLength;
	new_string.mCharacters = new char[new_string.mLength + 1];
	memcpy(new_string.mCharacters, mCharacters, mLength);
	memcpy(new_string.mCharacters + mLength, inOther.mCharacters, inOther.mLength);
	new_string.mCharacters[new_string.mLength] = '\0';

	new_string.mHash = gHash(new_string.mCharacters, new_string.mLength);

	return new_string;
}

String& String::operator+=(const String& inOther)
{
	*this = *this + inOther;
	return *this;
}

bool String::Contains(const char* inCharacters) const
{
	gAssert(inCharacters != nullptr);
	for (uint64 i = 0; i < mLength; i++)
	{
		if (mCharacters[i] == inCharacters[0])
		{
			bool found = true;
			for (uint64 j = 1; j < strlen(inCharacters); j++)
			{
				if (mCharacters[i + j] != inCharacters[j])
				{
					found = false;
					break;
				}
			}

			if (found)
				return true;
		}
	}
	return false;
}

void String::SetCharacters(const char* inCharacters, uint32 inLength)
{
	gAssert(mCharacters == nullptr);

	mLength = inLength;
	mCharacters = new char[mLength + 1];
	memcpy(mCharacters, inCharacters, mLength + 1);

	mHash = gHash(mCharacters, mLength);
}

uint64_t gRotl64(uint64 inX, int8 inR)
{
	return (inX << inR) | (inX >> (64 - inR));
}

uint64_t gFmix64(uint64 inVar)
{
	inVar ^= inVar >> 33;
	inVar *= 0xff51afd7ed558ccd;
	inVar ^= inVar >> 33;
	inVar *= 0xc4ceb9fe1a85ec53;
	inVar ^= inVar >> 33;

	return inVar;
}

// MurmurHash3 64-bit variant
uint64 gHash(const char* inCharacters, uint32 inLength, uint64 inSeed)
{
    const uint8_t* data = (const uint8_t*)inCharacters;
    const int nblocks = inLength / 8;

    uint64 h1 = inSeed;

    constexpr uint64 c1 = 0x87c37b91114253d5;
    constexpr uint64 c2 = 0x4cf5ad432745937f;

    // Body
    const uint64* blocks = (const uint64*)(data);

    for (int i = 0; i < nblocks; i++)
    {
        uint64 k1 = blocks[i];

        k1 *= c1;
        k1 = gRotl64(k1, 31);
        k1 *= c2;
        h1 ^= k1;

        h1 = gRotl64(h1, 27);
        h1 = h1 * 5 + 0x52dce729;
    }

    // Tail
    const uint8_t* tail = (const uint8_t*)(data + nblocks * 8);

    uint64 k1 = 0;

    switch (inLength & 7)
    {
    case 7: k1 ^= ((uint64)tail[6]) << 48;
    case 6: k1 ^= ((uint64)tail[5]) << 40;
    case 5: k1 ^= ((uint64)tail[4]) << 32;
    case 4: k1 ^= ((uint64)tail[3]) << 24;
    case 3: k1 ^= ((uint64)tail[2]) << 16;
    case 2: k1 ^= ((uint64)tail[1]) << 8;
    case 1: k1 ^= ((uint64)tail[0]) << 0;
        k1 *= c1;
        k1 = gRotl64(k1, 31);
        k1 *= c2;
        h1 ^= k1;
    }

    // Finalization
    h1 ^= inLength;
    h1 = gFmix64(h1);

    return h1;
}

// MurmurHash3 128-bit variant
Pair<uint64, uint64> gHash_128(const char* inCharacters, uint32 inLength, uint64 inSeed)
{
	gAssert(inLength > 0);

	const uint8* data = (const uint8*)inCharacters;
	const int nblocks = inLength / 16;

	uint64 h1 = inSeed;
	uint64 h2 = inSeed;

	constexpr uint64 c1 = 0x87c37b91114253d5;
	constexpr uint64 c2 = 0x4cf5ad432745937f;

	// Body
	const uint64* blocks = (const uint64*)(data);

	for (int i = 0; i < nblocks; i++)
	{
		uint64 k1 = blocks[i * 2 + 0];
		uint64 k2 = blocks[i * 2 + 1];

		k1 *= c1;
		k1 = gRotl64(k1, 31);
		k1 *= c2;
		h1 ^= k1;

		h1 = gRotl64(h1, 27);
		h1 += h2;
		h1 = h1 * 5 + 0x52dce729;

		k2 *= c2;
		k2 = gRotl64(k2, 33);
		k2 *= c1;
		h2 ^= k2;

		h2 = gRotl64(h2, 31);
		h2 += h1;
		h2 = h2 * 5 + 0x38495ab5;
	}

	// Tail
	const uint8_t* tail = (const uint8_t*)(data + nblocks * 16);

	uint64 k1 = 0;
	uint64 k2 = 0;

	switch (inLength & 15)
	{
	case 15: k2 ^= ((uint64)tail[14]) << 48;
	case 14: k2 ^= ((uint64)tail[13]) << 40;
	case 13: k2 ^= ((uint64)tail[12]) << 32;
	case 12: k2 ^= ((uint64)tail[11]) << 24;
	case 11: k2 ^= ((uint64)tail[10]) << 16;
	case 10: k2 ^= ((uint64)tail[9]) << 8;
	case 9: k2 ^= ((uint64)tail[8]) << 0;
		k2 *= c2;
		k2 = gRotl64(k2, 33);
		k2 *= c1;
		h2 ^= k2;

	case 8: k1 ^= ((uint64)tail[7]) << 56;
	case 7: k1 ^= ((uint64)tail[6]) << 48;
	case 6: k1 ^= ((uint64)tail[5]) << 40;
	case 5: k1 ^= ((uint64)tail[4]) << 32;
	case 4: k1 ^= ((uint64)tail[3]) << 24;
	case 3: k1 ^= ((uint64)tail[2]) << 16;
	case 2: k1 ^= ((uint64)tail[1]) << 8;
	case 1: k1 ^= ((uint64)tail[0]) << 0;
		k1 *= c1;
		k1 = gRotl64(k1, 31);
		k1 *= c2;
		h1 ^= k1;
	}

	// Finalization
	h1 ^= inLength;
	h2 ^= inLength;

	h1 += h2;
	h2 += h1;

	h1 = gFmix64(h1);
	h2 = gFmix64(h2);

	h1 += h2;
	h2 += h1;

	return Pair(h1, h2);
}