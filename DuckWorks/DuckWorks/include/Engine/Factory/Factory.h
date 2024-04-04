#pragma once
#include "Core/CoreBase.h"

class Entity;
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

	[[nodiscard]] taFactoryType* CreateClass(const String& inClassName);

	[[nodiscard]] const Array<String>& GetClassNames() const;

private:
	HashMap<String, ConstructorFunction> mClassConstructors;
	Array<String> mKeyList;
};

extern Factory<Entity> gEntityFactory;
extern Factory<DebugUIWindow> gDebugUIWindowFactory;

class ComponentFactory : public Factory<ComponentBase>
{
public:
	using ComponentCreationFunction = std::function<ComponentBase*(entt::registry&, entt::entity)>;
	using ComponentGetFunction = std::function<ComponentBase*(entt::registry&, entt::entity)>;
	using HasComponentFunction = std::function<bool(entt::registry&, entt::entity)>;

	template<typename taType>
	void RegisterClass(const String& inClassName)
	{
		Factory<ComponentBase>::RegisterClass<taType>(inClassName);

		mComponentCreationFunctions[inClassName] = [](entt::registry& inRegistry, entt::entity inEntity)
		{
			return &inRegistry.emplace<taType>(inEntity);
		};

		mComponentGetFunctions[inClassName] = [](entt::registry& inRegistry, entt::entity inEntity)
		{
			return &inRegistry.get<taType>(inEntity);
		};

		mHasComponentFunctions[inClassName] = [](entt::registry& inRegistry, entt::entity inEntity)
		{
			return inRegistry.any_of<taType>(inEntity);
		};
	}

	ComponentBase* CreateComponent(const String& inClassName, entt::registry& inRegistry, entt::entity inEntity)
	{
		return mComponentCreationFunctions[inClassName](inRegistry, inEntity);
	}

	ComponentBase* GetComponent(const String& inClassName, entt::registry& inRegistry, entt::entity inEntity)
	{
		return mComponentGetFunctions[inClassName](inRegistry, inEntity);
	}

	bool HasComponent(const String& inClassName, entt::registry& inRegistry, entt::entity inEntity)
	{
		return mHasComponentFunctions[inClassName](inRegistry, inEntity);
	}

private:
	HashMap<String, ComponentCreationFunction> mComponentCreationFunctions;
	HashMap<String, ComponentGetFunction> mComponentGetFunctions;
	HashMap<String, HasComponentFunction> mHasComponentFunctions;
};

extern ComponentFactory gComponentFactory;

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
	mKeyList.emplace_back(inClassName);
}

template<typename taFactoryType>
taFactoryType* Factory<taFactoryType>::CreateClass(const String& inClassName)
{
	gAssert(mClassConstructors.contains(inClassName), "Class not registered!");
	return mClassConstructors[inClassName]();
}

template<typename taFactoryType>
const Array<String>& Factory<taFactoryType>::GetClassNames() const
{
	return mKeyList;
}
