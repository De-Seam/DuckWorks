#include "Precomp.h"
#include "Engine/Resources/ResourceTypes/BaseResource.h"

RTTI_CLASS_DEFINITION(BaseResource, StandardAllocator)

Json BaseResource::Serialize()
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mFile);

	return json;
}

void BaseResource::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	JSON_LOAD(inJson, mFile);
}
