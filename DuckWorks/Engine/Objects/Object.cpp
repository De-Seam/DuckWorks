#include <Engine/Objects/Object.h>

// Engine includes
#include <Engine/Engine.h>
#include <Engine/Objects/ObjectManager.h>

Object::Object(const DC::GUID& inGUID) :
	mGUID(inGUID)
{
	gEngine->GetManager<ObjectManager>().RegisterObject(*this);
}

Object::Object()
{
	gEngine->GetManager<ObjectManager>().RegisterObject(*this);
}

Object::~Object()
{
	gEngine->GetManager<ObjectManager>().UnregisterObject(*this);
}

void Object::SetGUID(const DC::GUID& inGUID)
{
	gEngine->GetManager<ObjectManager>().UnregisterObject(*this);
	mGUID = inGUID;
	gEngine->GetManager<ObjectManager>().RegisterObject(*this);
}

Json Object::Serialize() const
{
	Json json = RTTIRefClass::Serialize();
	json["GUID"] = mGUID;
	return json;
}

void Object::Deserialize(const Json& inJson)
{
	RTTIRefClass::Deserialize(inJson);
	const DC::GUID& new_guid = inJson["GUID"];

	if (new_guid != mGUID)
		SetGUID(mGUID);
}
