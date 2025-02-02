#include "Precomp.h"
#include "Engine/Resources/ResourceTypes/BaseResource.h"

// Std includes
#include <fstream>

RTTI_CLASS_DEFINITION(BaseResource, StandardAllocator)

Json BaseResource::Serialize()
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mFile);

	return json;
}

void BaseResource::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	JSON_LOAD(inJson, mFile);
}

void BaseResource::Reload()
{
	LoadFromFile(mFile);
}

String BaseResource::sReadFile(const String& inFile)
{
	std::ifstream file(inFile);
	if (!file.is_open())
	{
		gLog(ELogType::Error, "Failed to open file: %s", inFile.c_str());
		return {};
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}
