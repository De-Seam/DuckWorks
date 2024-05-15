#include "Precomp.h"
#include "Engine/Entity/Entity.h"
#include "Engine/Collision/CollisionWorld.h"
#include "Engine/World/World.h"

RTTI_CLASS_DEFINITION(Entity)

Json Entity::Serialize()
{
	Json json = Base::Serialize();

	return json;
}

void Entity::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);
}

Entity::~Entity() {}

void Entity::Destroy()
{
	GetWorld()->DestroyEntity(this);
}

Array<MutexReadProtectedPtr<EntityComponent>> Entity::GetComponentsOfType(UID inComponentUID)
{
	ScopedMutexReadLock lock(mEntityComponentsMutex);
	Array<MutexReadProtectedPtr<EntityComponent>> return_array;

	Array<Handle<EntityComponent>>& components = mEntityComponents[inComponentUID];
	for (Handle<EntityComponent>& component : components)
		return_array.emplace_back(gEntityComponentManager.GetComponent(component, inComponentUID));

	return return_array;
}

bool Entity::HasComponent(UID inComponentUID)
{
	ScopedMutexReadLock lock(mEntityComponentsMutex);

	auto it = mEntityComponents.find(inComponentUID);
	if (it == mEntityComponents.end())
		return false;

	return !it->second.empty();
}

void Entity::SetTransform(const fm::Transform2D& inTransform)
{
	ScopedMutexWriteLock lock(mTransformMutex);
	mTransform = inTransform;
}

void Entity::SetPosition(const fm::vec2& inPosition)
{
	ScopedMutexWriteLock lock(mTransformMutex);
	mTransform.position = inPosition;
}

void Entity::SetHalfSize(const fm::vec2& inHalfSize)
{
	ScopedMutexWriteLock lock(mTransformMutex);
	mTransform.halfSize = inHalfSize;
}

void Entity::SetRotation(float inRotation)
{
	ScopedMutexWriteLock lock(mTransformMutex);
	mTransform.rotation = inRotation;
}

fm::Transform2D Entity::GetTransform()
{
	ScopedMutexWriteLock lock(mTransformMutex);
	return mTransform;
}

fm::vec2 Entity::GetPosition()
{
	ScopedMutexWriteLock lock(mTransformMutex);
	return mTransform.position;
}

fm::vec2 Entity::GetHalfSize()
{
	ScopedMutexWriteLock lock(mTransformMutex);
	return mTransform.halfSize;
}

float Entity::GetRotation()
{
	ScopedMutexWriteLock lock(mTransformMutex);
	return mTransform.rotation;
}
