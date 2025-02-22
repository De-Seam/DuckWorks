#pragma once
#include <DuckCore/Containers/HashMap.h>
#include <DuckCore/Containers/HashSet.h>
#include <DuckCore/Managers/Manager.h>

#include <Engine/Objects/Object.h>

class ObjectManager : public DC::Manager
{
	MANAGER_BASE_CLASS(ObjectManager)
public:

private:
	void RegisterObject(Object& inObject);
	void UnregisterObject(Object& inObject);

	DC::HashSet<Object*> mObjects;

	friend class Object;
};
