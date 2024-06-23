#pragma once
// Core includes
#include "Core/CoreBase.h"

// Engine includes
#include "Engine/Entity/Entity.h"
#include "Engine/Entity/Components/EntityComponent.h"

class Entity;
class EntityComponent;
class World;
class DebugUIWindow;
class ComponentBase;

template<typename taFactoryType>
class Factory
{
public:
	using ConstructorFunction = std::function<taFactoryType*(const typename taFactoryType::ConstructParameters&)>;

	template<typename taType>
	void RegisterClass(const String& inClassName);

	[[nodiscard]] taFactoryType* CreateClassFromJsonAndDeserialize(const Json& inJson, const typename taFactoryType::ConstructParameters& inConstructParameters = {});
	[[nodiscard]] taFactoryType* CreateClass(const String& inClassName, const typename taFactoryType::ConstructParameters& inConstructParameters = {});

	[[nodiscard]] UID GetRTTIUID(const String& inClassName) const;

	[[nodiscard]] const Array<String>& GetClassNames() const;

private:
	HashMap<String, ConstructorFunction> mClassConstructors;
	HashMap<String, UID> mClassUIDs;
	Array<String> mKeyList;
};

extern Factory<EntityComponent> gEntityComponentFactory;
extern Factory<Entity> gEntityFactory;
extern Factory<DebugUIWindow> gDebugUIWindowFactory;

template<typename taFactoryType>
template<typename taType>
void Factory<taFactoryType>::RegisterClass(const String& inClassName)
{
	gAssert(!mClassConstructors.contains(inClassName), "Class already registered!");
	ConstructorFunction func = [](const typename taFactoryType::ConstructParameters& inConstructParameters)
	{
		gLog("Factory created class %s", taType::sGetClassName());
		typename taType::ConstructParameters construct_parameters = {inConstructParameters};
		return taType::sNewInstance(construct_parameters);
	};
	mClassConstructors[inClassName] = func;
	mClassUIDs[inClassName] = taType::sGetRTTIUID();
	mKeyList.emplace_back(inClassName);
}

template<typename taFactoryType>
taFactoryType* Factory<taFactoryType>::CreateClassFromJsonAndDeserialize(const Json& inJson, const typename taFactoryType::ConstructParameters& inConstructParameters)
{
	taFactoryType* return_ptr = CreateClass(inJson["ClassName"], inConstructParameters);
	return_ptr->Deserialize(inJson);
	return return_ptr;
}

template<typename taFactoryType>
taFactoryType* Factory<taFactoryType>::CreateClass(const String& inClassName, const typename taFactoryType::ConstructParameters& inConstructParameters)
{
	gAssert(mClassConstructors.contains(inClassName), "Class not registered!");
	return mClassConstructors[inClassName](inConstructParameters);
}

template<typename taFactoryType>
UID Factory<taFactoryType>::GetRTTIUID(const String& inClassName) const
{
	return mClassUIDs.at(inClassName);
}

template<typename taFactoryType>
const Array<String>& Factory<taFactoryType>::GetClassNames() const
{
	return mKeyList;
}
