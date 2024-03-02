#pragma once
#include "Core/CoreBase.h"

class Entity;
class World;

class EntityFactory
{
public:
	using ConstructorFunction = std::function<SharedPtr<Entity>(World* inWorld)>;

	template<typename taType>
	void RegisterEntity(const String& inClassName);

	[[nodiscard]] SharedPtr<Entity> CreateEntity(const String& inClassName, World* inWorld);

	[[nodiscard]] Array<String> GetEntityNames() const;

private:
	HashMap<String, ConstructorFunction> mEntityConstructors;
	Array<String> mKeyList;
};

extern EntityFactory gEntityFactory;

template<typename taType>
void EntityFactory::RegisterEntity(const String& inClassName)
{
	gAssert(!mEntityConstructors.contains(inClassName), "Entity already registered!");
	mEntityConstructors[inClassName] = [](World* inWorld) { return SharedPtr<Entity>(new taType(inWorld)); };
	mKeyList.push_back(inClassName);
}
