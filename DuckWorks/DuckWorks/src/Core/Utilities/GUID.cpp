#include "Precomp.h"
#include "Core/Utilities/GUID.h"

// Std includes
#include <random>

static std::random_device sRandomDevice;
static std::mt19937_64 sEngine(sRandomDevice());
static std::uniform_int_distribution<uint64> sUniformDistribution;

GUID::GUID(const String& inGUIDString)
{
	uint32 parts[4] = {0};

	if (sscanf_s(inGUIDString.c_str(), "%4x-%4x-%4x-%4x", &parts[0], &parts[1], &parts[2], &parts[3]) != 4)
	{
		gLog(ELogType::Error, "Invalid GUID format!");
		gAssert(false, "Invalid GUID format!");
		return;
	}

	mGUID |= (SCast<uint64>(parts[0]) << 48);
	mGUID |= (SCast<uint64>(parts[1]) << 32);
	mGUID |= (SCast<uint64>(parts[2]) << 16);
	mGUID |= (SCast<uint64>(parts[3]));
}

GUID GUID::sCreate()
{
	GUID guid;
	guid.mGUID = sUniformDistribution(sEngine);
	return guid;
}

GUID GUID::sCombine(const GUID& inGUID1, const GUID& inGUID2, int32 inSalt)
{
	GUID guid;
	guid.mGUID = inGUID1.mGUID ^ inGUID2.mGUID ^ inSalt;
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
	outVariable = GUID(inJson.get<String>());
}
