#include "Precomp.h"
#include "Engine/Entity/EntityFactory.h"

EntityFactory gEntityFactory = {};

SharedPtr<Entity> EntityFactory::CreateEntity(const String& inClassName, World* inWorld)
{
	gAssert(mEntityConstructors.contains(inClassName), "Entity not registered!");
	return mEntityConstructors[inClassName](inWorld);
}

Array<String> EntityFactory::GetEntityNames() const
{
	return mKeyList;
}
