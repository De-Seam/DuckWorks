#include "Precomp.h"
#include "Engine/Entity/Entity.h"

// Engine includes
#include "Engine/Collision/CollisionWorld.h"
#include "Engine/Engine/Engine.h"
#include "Engine/Entity/EntityMessages.h"
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
	JSON_SAVE(json, mUpdateFrequency);

	Json& json_components = json["Components"];
	LoopOverAllComponents([&](EntityComponent& inComponent)
	{
		json_components.emplace_back(&inComponent);
	});

	return json;
}

void Entity::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	JSON_LOAD(inJson, mName);
	JSON_LOAD(inJson, mPosition);
	JSON_LOAD(inJson, mRotation);
	if (inJson.contains("mUpdateFrequency"))
	{
		int update_frequency = inJson["mUpdateFrequency"];
		if (update_frequency >= 0 && update_frequency <= SCast<int>(EUpdateFrequency::None))
			SetUpdateFrequency(SCast<EUpdateFrequency>(update_frequency));
	}

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

Entity::Entity(const ConstructParameters& inConstructParameters)
	: Base(inConstructParameters)
{
	mWorld = inConstructParameters.mWorld;
	mName = inConstructParameters.mName;

	gAssert(mWorld != nullptr, "World is nullptr!");
	RegisterMessageListener(mWorld, &World::OnEntityUpdateFrequencyChanged);
}

Entity::~Entity()
{
	mEntityComponents.clear();

	UnregisterMessageListener(mWorld, &World::OnEntityUpdateFrequencyChanged);
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

	Array<Ref<EntityComponent>>& components = mEntityComponents[inComponentUID];
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

	Array<Ref<EntityComponent>>& components = mEntityComponents[inEntityComponent->GetRTTIUID()];
	for (auto it = components.rbegin(); it != components.rend(); it++)
	{
		EntityComponent* component = *it;
		if (component == inEntityComponent)
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

	Array<Ref<EntityComponent>>& components = mEntityComponents[inComponentUID];
	for (EntityComponent* component : components)
		inFunction(*component);
}

void Entity::LoopOverAllComponents(const Function<void(EntityComponent& inComponent)>& inFunction)
{
	gAssert(gIsMainThread());

	for (auto& pair : mEntityComponents)
	{
		Array<Ref<EntityComponent>>& components = pair.second;
		for (EntityComponent* component : components)
			inFunction(*component);
	}
}

void Entity::SetUpdateFrequency(EUpdateFrequency inUpdateFrequency)
{
	MsgEntityUpdateFrequencyChanged::ConstructParameters construct_parameters;
	construct_parameters.mEntity = this;
	construct_parameters.mOldUpdateFrequency = mUpdateFrequency;
	construct_parameters.mNewUpdateFrequency = inUpdateFrequency;
	mUpdateFrequency = inUpdateFrequency;

	MsgEntityUpdateFrequencyChanged message(construct_parameters);

	SendMessage(message);
}

void Entity::SetPosition(const Vec2& inPosition)
{
	gAssert(gIsMainThread());

	MsgPostEntityPositionUpdated::ConstructParameters post_construct_params;
	post_construct_params.mEntity = this;
	MsgPostEntityPositionUpdated post_update_msg(post_construct_params);
	post_update_msg.mOldPosition = mPosition;

	MsgPreEntityPositionUpdated::ConstructParameters pre_construct_params;
	pre_construct_params.mEntity = this;
	MsgPreEntityPositionUpdated pre_update_msg(pre_construct_params);
	pre_update_msg.mNewPosition = inPosition;
	SendMessage(pre_update_msg);

	mPosition = pre_update_msg.mNewPosition;

	post_update_msg.mNewPosition = mPosition;

	SendMessage(post_update_msg);
}

void Entity::SetRotation(float inRotation)
{
	gAssert(gIsMainThread());

	MsgPostEntityRotationUpdated::ConstructParameters post_construct_params;
	post_construct_params.mEntity = this;
	MsgPostEntityRotationUpdated post_update_msg(post_construct_params);
	post_update_msg.mOldRotation = mRotation;

	MsgPreEntityRotationUpdated::ConstructParameters pre_construct_params;
	pre_construct_params.mEntity = this;
	MsgPreEntityRotationUpdated pre_update_msg(pre_construct_params);
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
