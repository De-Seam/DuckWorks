#pragma once
#include "Core/CoreBase.h"

class Entity;
class World;
class DebugUIWindow;

template<typename taFactoryType>
class Factory
{
public:
	using ConstructorFunction = std::function<SharedPtr<taFactoryType>(void)>;

	template<typename taType>
	void RegisterClass(const String& inClassName);

	[[nodiscard]] SharedPtr<taFactoryType> CreateClass(const String& inClassName);

	[[nodiscard]] const Array<String>& GetClassNames() const;

private:
	HashMap<String, ConstructorFunction> mClassConstructors;
	Array<String> mKeyList;
};

extern Factory<Entity> gEntityFactory;
extern Factory<DebugUIWindow> gDebugUIWindowFactory;


template<typename taFactoryType>
template<typename taType>
void Factory<taFactoryType>::RegisterClass(const String& inClassName)
{
	gAssert(!mClassConstructors.contains(inClassName), "Class already registered!");
	mClassConstructors[inClassName] = []() { return std::make_shared<taType>(); };
	mKeyList.push_back(inClassName);
}

template<typename taFactoryType>
SharedPtr<taFactoryType> Factory<taFactoryType>::CreateClass(const String& inClassName)
{
	gAssert(mClassConstructors.contains(inClassName), "Class not registered!");
	return mClassConstructors[inClassName]();
}

template<typename taFactoryType>
const Array<String>& Factory<taFactoryType>::GetClassNames() const
{
	return mKeyList;
}
