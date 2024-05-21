#include "Precomp.h"
#include "Engine/Entity/Entity.h"

// Engine includes
#include "Engine/Collision/CollisionWorld.h"
#include "Engine/Factory/Factory.h"
#include "Engine/World/World.h"

RTTI_CLASS_DEFINITION(Entity)

Json Entity::Serialize()
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mTransform);

	const Array<String>& component_names = gEntityComponentFactory.GetClassNames();
	Json& json_component = json["Components"];
	for (const String& component_name : component_names)
	{
		UID rtti_uuid = gEntityComponentFactory.GetRTTIUID(component_name);
		LoopOverComponents(rtti_uuid, [&](EntityComponent& inComponent)
		{
			json_component[component_name] = inComponent.Serialize();
		});
	}

	return json;
}

void Entity::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	JSON_TRY_LOAD(inJson, mTransform);

	if (inJson.contains("Components"))
	{
		const Json& json_components = inJson["Components"];

		const Array<String>& component_names = gEntityComponentFactory.GetClassNames();
		for (const String& component_name : component_names)
		{
			if (!json_components.contains(component_name))
				continue;

			gEntityComponentFactory.AddComponent(this, component_name);
			MutexReadProtectedPtr<EntityComponent> component = GetLastComponentOfType(gEntityComponentFactory.GetRTTIUID(component_name));
			component->Deserialize(json_components[component_name]);
		}
	}
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

MutexReadProtectedPtr<EntityComponent> Entity::GetLastComponentOfType(UID inRTTIUUID)
{
	ScopedMutexReadLock lock(mEntityComponentsMutex);

	Array<Handle<EntityComponent>>& components = mEntityComponents[inRTTIUUID];

	gAssert(!components.empty(), "Component not found!");
	return gEntityComponentManager.GetComponent(components.back(), inRTTIUUID);
}

bool Entity::HasComponent(UID inComponentUID)
{
	ScopedMutexReadLock lock(mEntityComponentsMutex);

	auto it = mEntityComponents.find(inComponentUID);
	if (it == mEntityComponents.end())
		return false;

	return !it->second.empty();
}

void Entity::LoopOverComponents(UID inComponentUID, const Function<void(EntityComponent& inComponent)>& inFunction)
{
	ScopedMutexReadLock lock(mEntityComponentsMutex);

	Array<Handle<EntityComponent>>& component_handles = mEntityComponents[inComponentUID];
	for (Handle<EntityComponent>& component_handle : component_handles)
	{
		MutexReadProtectedPtr<EntityComponent> component = gEntityComponentManager.GetComponent(component_handle, inComponentUID);
		inFunction(*component.Get());
	}
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
