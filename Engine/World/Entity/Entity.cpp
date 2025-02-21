#include <Engine/World/Entity/Entity.h>

using namespace DC;

void Entity::OnAddedToScene(Scene& aScene)
{
	gAssert(mScene == nullptr, "Entity is already in a Scene.");
	mScene = &aScene;
}

void Entity::OnRemovedFromScene(Scene& aScene)
{
	gAssert(mScene == &aScene, "Entity is not part of this Scene.");
	mScene = nullptr;
}
