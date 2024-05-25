#include "Precomp.h"
#include "Core/Utilities/RTTI.h"

UID RTTIBaseClass::sRTTIBaseClassRTTIUID;

Json RTTIBaseClass::Serialize()
{
	Json json;
	json["ClassName"] = GetClassName();
	json["mGUID"] = GetGUID().ToString();
	return json;
}

void RTTIBaseClass::Deserialize(const Json& inJson)
{
	if (inJson.contains("mGUID"))
	{
		const String& guid_string = inJson["mGUID"];
		SetGUID(GUID(guid_string));
	}
}
