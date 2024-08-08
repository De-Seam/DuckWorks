#include "Precomp.h"
#include "Core/RTTI/RTTI.h"

Json RTTIClass::Serialize() const
{
	Json json;
	json["ClassName"] = sGetRTTI().GetClassName();
	JSON_SAVE(json, mGUID);
	return json;
}

void RTTIClass::Deserialize(const Json& inJson)
{
	gAssert(inJson["ClassName"] == sGetRTTI().GetClassName(), "Invalid class name!");
	JSON_LOAD(inJson, mGUID);
}
