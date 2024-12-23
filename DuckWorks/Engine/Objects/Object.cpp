#include <Engine/Objects/Object.h>

// Engine includes
#include <Engine/Engine.h>
#include <Engine/Objects/ObjectManager.h>

using namespace DC;

Object::Object(const GUID& inGUID) :
	mGUID(inGUID)
{
	gEngine->GetManager<ObjectManager>().RegisterObject(*this);
}

Object::~Object()
{
	if (gEngine == nullptr)
		return;
	
	if (ObjectManager* object_manager = gEngine->FindManager<ObjectManager>())
		object_manager->UnregisterObject(*this);
}
