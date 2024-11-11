#pragma once
// Core includes
#include <DuckCore/Containers/HashMap.h>
#include <DuckCore/Utilities/GUID.h>

// Engine includes
#include <Engine/Manager.h>

class Object;

class ObjectManager : public Manager
{
	RTTI_CLASS(ObjectManager, Manager)
public:

private:
	void RegisterObject(Object& inObject);
	void UnregisterObject(Object& inObject);

	DC::HashMap<DC::GUID, Object*> mObjects;

	friend class Object;
};