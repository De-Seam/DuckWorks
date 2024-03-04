#pragma once
#include "Core/CoreBase.h"

class Entity;
class World;
class DebugUIWindow;

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

template<typename taFactoryType, typename... taArgs>
template<typename taType>
void Factory<taFactoryType, taArgs...>::RegisterClass(const String& inClassName)
{
	gAssert(!mClassConstructors.contains(inClassName), "Class already registered!");
	mClassConstructors[inClassName] = [](taArgs&&... inArgs)
	{
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
