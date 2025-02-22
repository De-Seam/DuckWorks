#pragma once
#include <Engine/Objects/Object.h>

class Scene;

class Entity : public Object
{
	RTTI_CLASS(Entity, Object)
public:
	void OnAddedToScene(Scene& aScene);
	void OnRemovedFromScene(const Scene& aScene);

private:
	Scene* mScene = nullptr; // The scene this Entity is in.
};