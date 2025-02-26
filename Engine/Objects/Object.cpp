#include <Engine/Objects/Object.h>

// Engine includes
#include <DuckCore/Managers/Managers.h>

#include <Engine/Engine.h>
#include <Engine/Objects/ObjectManager.h>

using namespace DC;

Object::Object(const GUID& inGUID) :
	mGUID(inGUID)
{
	if (ObjectManager* object_manager = Managers::sFind<ObjectManager>())
		object_manager->RegisterObject(*this);
}

Object::Object(const Json& aJson)
{
	mGUID = aJson["mGUID"];

	if (ObjectManager* object_manager = Managers::sFind<ObjectManager>())
		object_manager->RegisterObject(*this);
}

Object::~Object()
{
	if (ObjectManager* object_manager = Managers::sFind<ObjectManager>())
		object_manager->UnregisterObject(*this);
}

Json Object::ToJson() const
{
	Json json;
	json["mGUID"] = mGUID;
	return json;
}
