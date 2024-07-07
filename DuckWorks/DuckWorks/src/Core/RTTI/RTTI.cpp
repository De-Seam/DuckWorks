#include "Precomp.h"
#include "Core/RTTI/RTTI.h"

UID RTTIBaseClass::sRTTIBaseClassRTTIUID;

Json RTTIBaseClass::Serialize()
{
	Json json;
	json["ClassName"] = GetClassName();
	JSON_SAVE(json, mGUID);
	return json;
}

void RTTIBaseClass::Deserialize(const Json& inJson)
{
	gAssert(inJson["ClassName"] == GetClassName(), "Invalid class name!");
	JSON_LOAD(inJson, mGUID);
}
