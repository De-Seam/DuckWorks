#include <Engine/Objects/Object.h>

// Engine includes
#include <Engine/Objects/ObjectManager.h>

Object* ObjectManager::FindObject(const DC::GUID& inGUID)
{
	Object** obj_ptr = mObjects.Find(inGUID);
	if (obj_ptr != nullptr)
		return *obj_ptr;
	return nullptr;
}

void ObjectManager::RegisterObject(Object& inObject)
{
	if (inObject.GetGUID().IsValid())
	{
		gAssert(!mObjects.Contains(inObject.GetGUID()));
		mObjects[inObject.GetGUID()] = &inObject;
	}
}

void ObjectManager::UnregisterObject(Object& inObject)
{
	if (inObject.GetGUID().IsValid())
		gVerify(mObjects.Remove(inObject.GetGUID()));
}
