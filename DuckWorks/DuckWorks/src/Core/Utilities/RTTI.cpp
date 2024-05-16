#include "Precomp.h"
#include "Core/Utilities/RTTI.h"

ClassAllocator<RTTIBaseClass> RTTIBaseClass::sRTTIBaseClassClassAllocator;
UID RTTIBaseClass::sRTTIBaseClassRTTIUID;

Json RTTIBaseClass::Serialize() 
{ 
	Json json;
	json["ClassName"] = GetClassName();
	return json;
}
