#pragma once
#include "Core/CoreBase.h"

class Entity;
class World;
class DebugUIWindow;
class ComponentBase;

template<typename taFactoryType, typename... taArgs>
class Factory
{
public:
	using ConstructorFunction = std::function<SharedPtr<taFactoryType>(taArgs&&... inArgs)>;

	template<typename taType>
	void RegisterClass(const String& inClassName);

	[[nodiscard]] SharedPtr<taFactoryType> CreateClass(const String& inClassName, taArgs&&... inArgs);

	[[nodiscard]] const Array<String>& GetClassNames() const;

private:
	HashMap<String, ConstructorFunction> mClassConstructors;
	Array<String> mKeyList;
};

extern Factory<Entity, World*> gEntityFactory;
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

template<typename taFactoryType, typename... taArgs>
template<typename taType>
void Factory<taFactoryType, taArgs...>::RegisterClass(const String& inClassName)
{
	gAssert(!mClassConstructors.contains(inClassName), "Class already registered!");
	mClassConstructors[inClassName] = [](taArgs&&... inArgs)
	{
		gLog("Factory created class %s", taType::sGetClassName());
		return std::make_shared<taType>(std::forward<taArgs>(inArgs)...);
	};
	mKeyList.push_back(inClassName);
}

template<typename taFactoryType, typename... taArgs>
SharedPtr<taFactoryType> Factory<taFactoryType, taArgs...>::CreateClass(const String& inClassName, taArgs&&... inArgs)
{
	gAssert(mClassConstructors.contains(inClassName), "Class not registered!");
	return mClassConstructors[inClassName](std::forward<taArgs>(inArgs)...);
}

template<typename taFactoryType, typename... taArgs>
const Array<String>& Factory<taFactoryType, taArgs...>::GetClassNames() const
{
	return mKeyList;
}
