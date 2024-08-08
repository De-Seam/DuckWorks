#include "Precomp.h"
#include "Engine/Engine/BaseUserSettings.h"

Json BaseUserSettings::Serialize()
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mMaxFPS);
	JSON_SAVE(json, mWindowSize);
	JSON_SAVE(json, mWindowFlags);

	return json;
}

void BaseUserSettings::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	JSON_LOAD(inJson, mMaxFPS);
	JSON_LOAD(inJson, mWindowSize);
	JSON_LOAD(inJson, mWindowFlags);
}
