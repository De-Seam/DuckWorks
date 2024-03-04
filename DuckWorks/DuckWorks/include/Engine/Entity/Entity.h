#pragma once
#include "BaseEntity.h"


class Entity;

using EntityPtr = SharedPtr<Entity>;
using EntityWeakPtr = WeakPtr<Entity>;
/*
 * Entity rules:
 * An EntityPtr (SharedPtr<Entity>) is a reference to an entity which is always valid.
 * It is possible for an EntityPtr to reference an entity that has been destroyed, but the EntityPtr will still be valid.
 * An EntityWeakPtr (WeakPtr<Entity>) is a reference to an entity which may be invalid.
 */

class Entity : public BaseEntity
{
	RTTI_CLASS(Entity, BaseEntity)

public:
	Entity(World* inWorld)
		: BaseEntity(inWorld) {}

	virtual void BeginPlay() {}
	virtual void EndPlay() {}
	virtual void Update(float inDeltaTime) { (void)inDeltaTime; }

protected:
	WeakPtr<Entity> mThisWeakPtr;

	friend class World;
};
