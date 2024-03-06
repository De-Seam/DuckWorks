#include "Precomp.h"
#include "Engine/User/UserSettings.h"

Json BaseUserSettings::Serialize()
{
	Json json;
	json["mWindowSize"] = {mWindowSize.x, mWindowSize.y};
	JSON_SAVE(json, mWindowFlags);

	return json;
}

void BaseUserSettings::Deserialize(const Json& inJson)
{
	mWindowSize.x = inJson["mWindowSize"][0];
	mWindowSize.y = inJson["mWindowSize"][1];
	JSON_LOAD(inJson, mWindowFlags);
}
