#pragma once
// Core includes
#include <DuckCore/Containers/HashMap.h>
#include <DuckCore/Utilities/GUID.h>

// Engine includes
#include <Engine/Manager.h>
#include <Engine/Objects/Object.h>

class ObjectManager : public Manager
{
	RTTI_CLASS(ObjectManager, Manager)
public:
	Object* FindObject(const DC::GUID& inGUID);
	template<typename taType>
	taType* Find(const DC::GUID& inGUID);

private:
	void RegisterObject(Object& inObject);
	void UnregisterObject(Object& inObject);

	DC::HashMap<DC::GUID, Object*> mObjects;

	friend class Object;
};

template<typename taType>
taType* ObjectManager::Find(const DC::GUID& inGUID)
{
	Object* object = FindObject(inGUID);
	if (object == nullptr)
		return nullptr;
	return object->As<taType>();
}
