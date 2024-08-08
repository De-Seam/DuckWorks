#include "Precomp.h"
#include "Engine/Resources/ResourceTypes/BaseResource.h"

// Std includes
#include <fstream>

Json BaseResource::Serialize()
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mFile);

	return json;
}

void BaseResource::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	if (inJson["mFile"] != GetFileName())
	{
		gLog(ELogType::Error, "Resource::Deserialize should not be called with a different file than the current file!");
		return;
	}

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
