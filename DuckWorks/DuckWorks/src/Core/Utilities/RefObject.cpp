#include "Precomp.h"
#include "Core/Utilities/RefObject.h"

RTTI_CLASS_DEFINITION(RefObject)

Json RefObject::Serialize() 
{ 
	Json json = Base::Serialize(); 
	json["mGUID"] = GetGUID().ToString();
} 

void RefObject::Deserialize(const Json& inJson) 
{ 
	Base::Deserialize(inJson); 

	if (inJson.contains("mGUID"))
	{
		const String& guid_string = inJson["mGUID"];
		SetGUID(GUID(guid_string));
	}
}

// This is an invalid weak ref counter to be used when nullptr is passed. It always keeps 1 reference to itself to prevent it's destruction
RefObject::WeakRefCounter gInvalidWeakRefCounter = {false, 1};
