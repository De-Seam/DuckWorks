#include "Precomp.h"
#include "Engine/Resources/ResourceTypes/BaseResource.h"

Json BaseResource::Serialize() const
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
