#include <Engine/Resources/Resource.h>

// Engine includes
#include <Engine/Engine.h>
#include <Engine/Objects/ObjectManager.h>
#include <Engine/Resources/ResourceManager.h>

Json Resource::Serialize() const
{
	Json json = Base::Serialize();

	json["mFile"] = mFile;

	return json;
}

void Resource::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	// mFile should be set in the constructor, so we just assert that it is the same
	gAssert(inJson["mFile"] == mFile.CStr());
}
