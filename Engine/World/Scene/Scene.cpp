#include <Engine/World/Scene/Scene.h>

using namespace DC;

Scene::Scene(const GUID& aGUID) :
	Base(aGUID)
{}

Scene::Scene(const GUID& aGUID, const Json& aJson) :
	Base(aGUID)
{
	
}

void Scene::AddEntity(Ref<Entity>& aEntity)
{
	gAssert(!mEntities.Contains(aEntity));
	mEntities.Add(aEntity);
	aEntity->OnAddedToScene(*this);
}

void Scene::RemoveEntity(Entity& aEntity)
{
	mEntities.SwapRemoveFirstIf([&aEntity](const Entity* aItem) { return aItem == &aEntity; });
	aEntity.OnRemovedFromScene(*this);
}
