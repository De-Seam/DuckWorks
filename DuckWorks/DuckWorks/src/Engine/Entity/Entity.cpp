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
	Json& json_components = json["Components"];
	for (const String& component_name : component_names)
	{
		UID rtti_uuid = gEntityComponentFactory.GetRTTIUID(component_name);
		LoopOverComponents(rtti_uuid, [&](EntityComponent& inComponent)
		{
			Json json_component;
			json_component = inComponent.Serialize();
			json_components.emplace_back(json_component);
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

		for (const Json& json_component : json_components)
		{
			const String& class_name = json_component["ClassName"];
			UID rtti_uid = gEntityComponentFactory.GetRTTIUID(class_name);
			EntityComponent* component = nullptr;

			for (EntityComponent* current_component : mEntityComponents[rtti_uid])
			{
				if (json_component["mGUID"] == current_component->GetGUID().ToString())
				{
					component = current_component;
					break;
				}
			}

			if (component == nullptr)
				component = gEntityComponentFactory.AddComponent(this, class_name);

			component->Deserialize(json_component);
		}
	}
}

Entity::~Entity() {}

void Entity::Destroy()
{
	GetWorld()->DestroyEntity(this);
}

Array<EntityComponent*> Entity::GetComponentsOfType(UID inComponentUID)
{
	ScopedMutexReadLock lock(mEntityComponentsMutex);
	Array<EntityComponent*> return_array;

	Array<EntityComponent*>& components = mEntityComponents[inComponentUID];
	for (EntityComponent* component : components)
		return_array.emplace_back(component);

	return return_array;
}

int32 Entity::GetComponentCountOfType(UID inComponentUID)
{
	ScopedMutexReadLock lock(mEntityComponentsMutex);
	return SCast<int32>(mEntityComponents[inComponentUID].size());
}

bool Entity::HasComponent(UID inComponentUID)
{
	ScopedMutexReadLock lock(mEntityComponentsMutex);

	auto it = mEntityComponents.find(inComponentUID);
	if (it == mEntityComponents.end())
		return false;

	return !it->second.empty();
}

void Entity::RemoveComponent(EntityComponent* inEntityComponent)
{
	ScopedMutexWriteLock lock(mEntityComponentsMutex);

	Array<EntityComponent*>& components = mEntityComponents[inEntityComponent->GetRTTIUID()];
	for (auto it = components.rbegin(); it != components.rend(); it++)
	{
		if (*it == inEntityComponent)
		{
			components.erase(it.base());
			delete inEntityComponent;
			return;
		}
	}
	gAssert(false, "Component not found!");
}

void Entity::LoopOverComponents(UID inComponentUID, const Function<void(EntityComponent& inComponent)>& inFunction)
{
	ScopedMutexReadLock lock(mEntityComponentsMutex);

	Array<EntityComponent*>& components = mEntityComponents[inComponentUID];
	for (EntityComponent* component : components)
		inFunction(*component);
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
