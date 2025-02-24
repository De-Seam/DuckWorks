#pragma once
#include <Engine/Objects/Object.h>
#include <Engine/World/Entity/ECSEntity.h>

class Scene;

class Entity : public Object, public ECSEntity
{
	RTTI_CLASS(Entity, Object)
public:
	void OnAddedToScene(Scene& aScene);
	void OnRemovedFromScene(const Scene& aScene);

	void FromJson(const DC::Json& aJson);
	DC::Json ToJson() const;

private:
};