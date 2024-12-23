#pragma once
// Core includes
#include <DuckCore/Containers/HashMap.h>
#include <DuckCore/Utilities/GUID.h>

// Engine includes
#include <DuckCore/Containers/HashSet.h>
#include <Engine/Manager.h>
#include <Engine/Objects/Object.h>

class ObjectManager : public Manager
{
	RTTI_CLASS(ObjectManager, Manager)
public:

private:
	void RegisterObject(Object& inObject);
	void UnregisterObject(Object& inObject);

	DC::HashSet<Object*> mObjects;

	friend class Object;
};
