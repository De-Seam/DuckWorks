#include <Engine/Objects/Object.h>

// Engine includes
#include <Engine/Objects/ObjectManager.h>

void ObjectManager::RegisterObject(Object& inObject)
{
	gAssert(!mObjects.Contains(inObject.GetGUID()));
	mObjects[inObject.GetGUID()] = &inObject;
}

void ObjectManager::UnregisterObject(Object& inObject)
{
	gVerify(mObjects.Remove(inObject.GetGUID()));
}
