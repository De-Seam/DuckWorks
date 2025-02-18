#include <Engine/Objects/Object.h>

// Engine includes
#include <DuckCore/Manager/Managers.h>

#include <Engine/Engine.h>
#include <Engine/Objects/ObjectManager.h>

using namespace DC;

Object::Object(const GUID& inGUID) :
	mGUID(inGUID)
{
	if (ObjectManager* object_manager = Managers::sFind<ObjectManager>())
		object_manager->RegisterObject(*this);
}

Object::~Object()
{
	if (gEngine == nullptr)
		return;
	
	if (ObjectManager* object_manager = Managers::sFind<ObjectManager>())
		object_manager->UnregisterObject(*this);
}
