#include <Engine/Objects/Object.h>

// Engine includes
#include <Engine/Engine.h>
#include <Engine/Objects/ObjectManager.h>

Object::Object()
{
	gEngine->GetManager<ObjectManager>().RegisterObject(*this);
}

Object::~Object()
{
	gEngine->GetManager<ObjectManager>().UnregisterObject(*this);
}
