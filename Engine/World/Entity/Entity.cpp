#include <Engine/World/Entity/Entity.h>

#include <Engine/World/Scene/Scene.h>

using namespace DC;

void Entity::OnAddedToScene(Scene& aScene)
{
	gAssert(mScene == nullptr, "Entity is already in a Scene.");
	mScene = &aScene;
}

void Entity::OnRemovedFromScene(const Scene& aScene)
{
	gAssert(mScene == &aScene, "Entity is not part of this Scene.");
	mScene = nullptr;
}

void Entity::FromJson(const Json& aJson)
{
	const Json& json_components = aJson["Components"];
	for (const Json& json_component : json_components)
	{
		
	}
}

Json Entity::ToJson() const
{
	Json json;
	Json& json_components = json["Components"];
}
