#include <Core/Utilities/Assert.h>
#include <Core/Utilities/GUID.h>

// Std includes
#include <random>

static std::random_device sRandomDevice;
static std::mt19937_64 sEngine(sRandomDevice());
static std::uniform_int_distribution<uint64> sUniformDistribution;

GUID::GUID(const String& inGUIDString)
	: GUID(inGUIDString.c_str()) {}

GUID::GUID(const char* inChars) 
{
	uint32 parts[4] = {0};

	if (sscanf_s(inChars, "%4x-%4x-%4x-%4x", &parts[0], &parts[1], &parts[2], &parts[3]) != 4)
	{
		gAssert(false && "Invalid GUID format!");
		return;
	}

	mGUID |= (static_cast<uint64>(parts[0]) << 48);
	mGUID |= (static_cast<uint64>(parts[1]) << 32);
	mGUID |= (static_cast<uint64>(parts[2]) << 16);
	mGUID |= (static_cast<uint64>(parts[3]));
}

GUID GUID::sCreate()
{
	GUID guid;
	guid.mGUID = sUniformDistribution(sEngine);
	return guid;
}

String GUID::ToString() const
{
	char buffer[20]; // 16 hex digits + 3 dashes + 1 null terminator
	std::snprintf(buffer, sizeof(buffer), "%04x-%04x-%04x-%04x",
				(unsigned)((mGUID >> 48) & 0xFFFF),
				(unsigned)((mGUID >> 32) & 0xFFFF),
				(unsigned)((mGUID >> 16) & 0xFFFF),
				(unsigned)(mGUID & 0xFFFF));
	return {buffer};
}

void to_json(Json& outJson, const GUID& inVariable)
{
	outJson = inVariable.ToString();
}

void from_json(const Json& inJson, GUID& outVariable)
{
	outVariable = GUID(inJson.get<std::string>().c_str());
}