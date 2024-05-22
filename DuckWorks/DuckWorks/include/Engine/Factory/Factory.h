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
	using ConstructorFunction = std::function<taFactoryType*()>;

	template<typename taType>
	void RegisterClass(const String& inClassName);

	[[nodiscard]] taFactoryType* CreateClassFromJsonAndDeserialize(const Json& inJson);
	[[nodiscard]] taFactoryType* CreateClass(const String& inClassName);

	[[nodiscard]] UID GetRTTIUID(const String& inClassName) const;

	[[nodiscard]] const Array<String>& GetClassNames() const;

private:
	HashMap<String, ConstructorFunction> mClassConstructors;
	HashMap<String, UID> mClassUIDs;
	Array<String> mKeyList;
};

extern Factory<Entity> gEntityFactory;
extern Factory<DebugUIWindow> gDebugUIWindowFactory;

class EntityComponentFactory : public Factory<EntityComponent>
{
public:
	using ComponentAddFunction = std::function<EntityComponent*(const Ref<Entity>&)>;

	template<typename taType>
	void RegisterClass(const String& inClassName)
	{
		Factory<EntityComponent>::RegisterClass<taType>(inClassName);

		ComponentAddFunction func = ComponentAddFunction([](const Ref<Entity>& inEntity)
		{
			return inEntity->AddComponent<taType>();
		});

		mComponentAddFunctions[inClassName] = func;
	}

	EntityComponent* AddComponent(const Ref<Entity>& inEntity, const String& inClassName)
	{
		return mComponentAddFunctions[inClassName](inEntity);
	}

private:
	HashMap<String, ComponentAddFunction> mComponentAddFunctions;
};

extern EntityComponentFactory gEntityComponentFactory;

template<typename taFactoryType>
template<typename taType>
void Factory<taFactoryType>::RegisterClass(const String& inClassName)
{
	gAssert(!mClassConstructors.contains(inClassName), "Class already registered!");
	ConstructorFunction func = []()
	{
		gLog("Factory created class %s", taType::sGetClassName());
		return new taType;
	};
	mClassConstructors[inClassName] = func;
	mClassUIDs[inClassName] = taType::sGetRTTIUID();
	mKeyList.emplace_back(inClassName);
}

template<typename taFactoryType>
taFactoryType* Factory<taFactoryType>::CreateClassFromJsonAndDeserialize(const Json& inJson)
{
	taFactoryType* return_ptr = CreateClass(inJson["ClassName"]);
	return_ptr->Deserialize(inJson);
	return return_ptr;
}

template<typename taFactoryType>
taFactoryType* Factory<taFactoryType>::CreateClass(const String& inClassName)
{
	gAssert(mClassConstructors.contains(inClassName), "Class not registered!");
	return mClassConstructors[inClassName]();
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
