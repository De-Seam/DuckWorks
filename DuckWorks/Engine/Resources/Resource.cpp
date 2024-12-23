#include <Engine/Resources/Resource.h>

// Engine includes
#include <Engine/Engine.h>
#include <Engine/Files/FileManager.h>
#include <Engine/Objects/ObjectManager.h>
#include <Engine/Resources/ResourceManager.h>

using namespace DC;

Resource::Resource(const Json& inJson)
{
	const GUID& guid = inJson["mGUID"];
	SetGUID(guid);
}

Json Resource::ToJson() const
{
	Json json;
	json["ClassName"] = GetRTTI().GetClassName();
	json["mGUID"] = GetGUID();
	return json;
}
