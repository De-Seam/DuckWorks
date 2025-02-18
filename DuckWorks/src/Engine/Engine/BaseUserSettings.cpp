#include "Precomp.h"
#include "Engine/Engine/BaseUserSettings.h"

using namespace DC;

RTTI_CLASS_DEFINITION(BaseUserSettings, StandardAllocator)

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

	JSON_TRY_LOAD(inJson, mMaxFPS);
	JSON_TRY_LOAD(inJson, mWindowSize);
	JSON_TRY_LOAD(inJson, mWindowFlags);
}
