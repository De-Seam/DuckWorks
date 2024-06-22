#include "Precomp.h"
#include "Engine/Entity/Entity.h"

// Engine includes
#include "Core/RTTI/Messages.h"

#include "Engine/Collision/CollisionWorld.h"
#include "Engine/Factory/Factory.h"
#include "Engine/World/World.h"

RTTI_CLASS_DEFINITION(Entity, StandardAllocator)

Json Entity::Serialize()
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mName);
	JSON_SAVE(json, mTransform);

	Json& json_components = json["Components"];
	LoopOverAllComponents([&](EntityComponent& inComponent)
	{
		Json json_component;
		json_component = inComponent.Serialize();
		json_components.emplace_back(json_component);
	});

	return json;
}

void Entity::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	JSON_TRY_LOAD(inJson, mName);
	JSON_TRY_LOAD(inJson, mTransform);

	if (inJson.contains("Components"))
	{
		const Json& json_components = inJson["Components"];

		for (const Json& json_component : json_components)
		{
			const String& class_name = json_component["ClassName"];
			UID rtti_uid = gRTTIFactory.GetRTTIUID(class_name);
			EntityComponent* component = nullptr;
			GUID guid = GUID(json_component["mGUID"]);

			for (EntityComponent* current_component : mEntityComponents[rtti_uid])
			{
				if (guid == current_component->GetGUID())
				{
					component = current_component;
					break;
				}
			}

			if (component == nullptr)
			{
				EntityComponent::ConstructParameters parameters = {};
				parameters.mEntity = this;
				component = gCast<EntityComponent>(gRTTIFactory.CreateClass(class_name, parameters));
				gAssert(component != nullptr, "Invalid component class!");
				AddComponent(component);
			}

			component->Deserialize(json_component);
		}
	}
}

Entity::~Entity()
{
	// Delete and Clear all components
	LoopOverAllComponents([&](EntityComponent& inComponent)
	{
		inComponent.Delete();
	});
	mEntityComponents.clear();
}

void Entity::Destroy()
{
	GetWorld()->DestroyEntity(this);
}

void Entity::AddComponent(EntityComponent* inComponent)
{
	static GUID sBaseComponentGUID = GUID("52af-b8bb-1b48-d338");
	const GUID guid = GUID::sCombine(GetGUID(), sBaseComponentGUID, mEntityComponentSalt++);

#ifdef _DEBUG
	// Check if the component is already added
	LoopOverAllComponents([&guid](const EntityComponent& inExistingComponent)
	{
		if (inExistingComponent.GetGUID() == guid)
		{
			gLog(ELogType::Error, "Component with the same uuid already exists on entity");
			gAssert(false, "Component with the same uuid already exists on entity");
		}
	});
#endif // _DEBUG

	inComponent->SetGUID(guid);

	ScopedMutexWriteLock lock(mEntityComponentsMutex);
	mEntityComponents[inComponent->GetRTTIUID()].emplace_back(inComponent);
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

void Entity::LoopOverAllComponents(const Function<void(EntityComponent& inComponent)>& inFunction)
{
	ScopedMutexReadLock lock(mEntityComponentsMutex);

	for (auto& pair : mEntityComponents)
	{
		Array<EntityComponent*>& components = pair.second;
		for (EntityComponent* component : components)
			inFunction(*component);
	}
}

void Entity::SetTransform(const fm::Transform2D& inTransform)
{
	MsgPostEntityTransformUpdated post_update_msg;
	post_update_msg.mEntity = this;
	post_update_msg.mOldTransform = GetTransform();

	MsgPreEntityTransformUpdated pre_update_msg;
	pre_update_msg.mEntity = this;
	pre_update_msg.mNewTransform = inTransform;
	SendMessage(pre_update_msg);

	{
		ScopedMutexWriteLock lock(mTransformMutex);
		mTransform = pre_update_msg.mNewTransform;
		post_update_msg.mNewTransform = mTransform;
	}

	SendMessage(post_update_msg);
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
