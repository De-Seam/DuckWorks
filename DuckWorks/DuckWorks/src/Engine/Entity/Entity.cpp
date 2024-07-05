#include "Precomp.h"
#include "Engine/Entity/Entity.h"

// Core includes
#include "Core/RTTI/Messages.h"

// Engine includes
#include "Engine/Collision/CollisionWorld.h"
#include "Engine/Engine/Engine.h"
#include "Engine/Factory/Factory.h"
#include "Engine/Resources/ResourceManager.h"
#include "Engine/Threads/ThreadManager.h"
#include "Engine/World/World.h"

RTTI_CLASS_DEFINITION(Entity, StandardAllocator)

Json Entity::Serialize()
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mName);
	JSON_SAVE(json, mPosition);
	JSON_SAVE(json, mRotation);

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
	JSON_TRY_LOAD(inJson, mPosition);
	JSON_TRY_LOAD(inJson, mRotation);

	if (inJson.contains("Components"))
	{
		const Json& json_components = inJson["Components"];

		for (const Json& json_component : json_components)
		{
			const String& class_name = json_component["ClassName"];
			UID rtti_uid = gEntityComponentFactory.GetRTTIUID(class_name);
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
				component = gCast<EntityComponent>(gEntityComponentFactory.CreateClass(class_name, parameters));
				gAssert(component != nullptr, "Invalid component class!");
				AddComponent(component);
			}

			component->Deserialize(json_component);
		}
	}

	SetPosition(mPosition);
	SetRotation(mRotation);
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
	gAssert(gIsMainThread());

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

	mEntityComponents[inComponent->GetRTTIUID()].emplace_back(inComponent);
}

Array<EntityComponent*> Entity::GetComponentsOfType(UID inComponentUID)
{
	gAssert(gIsMainThread());
	Array<EntityComponent*> return_array;

	Array<EntityComponent*>& components = mEntityComponents[inComponentUID];
	for (EntityComponent* component : components)
		return_array.emplace_back(component);

	return return_array;
}

int32 Entity::GetComponentCountOfType(UID inComponentUID)
{
	gAssert(gIsMainThread());
	return SCast<int32>(mEntityComponents[inComponentUID].size());
}

bool Entity::HasComponent(UID inComponentUID)
{
	gAssert(gIsMainThread());

	auto it = mEntityComponents.find(inComponentUID);
	if (it == mEntityComponents.end())
		return false;

	return !it->second.empty();
}

void Entity::RemoveComponent(EntityComponent* inEntityComponent)
{
	gAssert(gIsMainThread());

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
	gAssert(gIsMainThread());

	Array<EntityComponent*>& components = mEntityComponents[inComponentUID];
	for (EntityComponent* component : components)
		inFunction(*component);
}

void Entity::LoopOverAllComponents(const Function<void(EntityComponent& inComponent)>& inFunction)
{
	gAssert(gIsMainThread());

	for (auto& pair : mEntityComponents)
	{
		Array<EntityComponent*>& components = pair.second;
		for (EntityComponent* component : components)
			inFunction(*component);
	}
}

void Entity::SetPosition(const Vec2& inPosition)
{
	gAssert(gIsMainThread());

	MsgPostEntityPositionUpdated post_update_msg;
	post_update_msg.mEntity = this;
	post_update_msg.mOldPosition = mPosition;

	MsgPreEntityPositionUpdated pre_update_msg;
	pre_update_msg.mEntity = this;
	pre_update_msg.mNewPosition = inPosition;
	SendMessage(pre_update_msg);

	mPosition = pre_update_msg.mNewPosition;

	post_update_msg.mNewPosition = mPosition;

	SendMessage(post_update_msg);
}

void Entity::SetRotation(float inRotation)
{
	gAssert(gIsMainThread());

	MsgPostEntityRotationUpdated post_update_msg;
	post_update_msg.mEntity = this;
	post_update_msg.mOldRotation = mRotation;

	MsgPreEntityRotationUpdated pre_update_msg;
	pre_update_msg.mEntity = this;
	pre_update_msg.mNewRotation = inRotation;
	SendMessage(pre_update_msg);

	mRotation = pre_update_msg.mNewRotation;

	post_update_msg.mNewRotation = mRotation;

	SendMessage(post_update_msg);
}

Vec2 Entity::GetPosition() const
{
	return mPosition;
}

float Entity::GetRotation() const
{
	return mRotation;
}
