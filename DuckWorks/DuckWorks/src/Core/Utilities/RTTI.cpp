#include "Precomp.h"
#include "Core/Utilities/RTTI.h"

UID RTTIBaseClass::sRTTIBaseClassRTTIUID;

Json RTTIBaseClass::Serialize() 
{ 
	Json json;
	json["ClassName"] = GetClassName();
	return json;
}
