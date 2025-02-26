#pragma once
#include <Engine/Objects/Object.h>
#include <Engine/World/Entity/ECSEntity.h>

class Scene;

class Entity : public Object, public ECSEntity
{
	RTTI_CLASS(Entity, Object)
public:
	Entity(Scene& aScene, DC::GUID aGUID = DC::GUID::sCreate());
	Entity(Scene& aScene, const DC::Json& aJson);
	virtual DC::Json ToJson() const override;
	virtual ~Entity() override;

private:
	DC::Json mThisJson;	// This is a temporary variable to store the JSON data of this entity for when we're removed from a Scene.
};