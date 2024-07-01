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
	JSON_SAVE(json, mTransform);
	JSON_SAVE(json, mLuaUpdateFile);

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

	if (inJson.contains("mLuaUpdateFile"))
	{
		const String& lua_update_file = inJson["mLuaUpdateFile"];
		if (lua_update_file.empty())
		{
			mLuaUpdateFile = "";
			mLuaUpdateResource = nullptr;
		}
		else if (gEngine.FileExists(lua_update_file.c_str()) && gIsValidLuaExtension(lua_update_file))
		{
			mLuaUpdateFile = lua_update_file;
			mLuaUpdateResource = gResourceManager.GetResource<LuaResource>(mLuaUpdateFile);
		}
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

void Entity::Update(float inDeltaTime)
{
	if (mLuaUpdateResource == nullptr)
		return;

	sol::state& lua = gEngine.GetLua();

	lua["Entity"] = this;
	lua["deltaTime"] = inDeltaTime;

	mLuaUpdateResource->RunScript(lua);
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

void Entity::SetTransform(const fm::Transform2D& inTransform)
{
	gAssert(gIsMainThread());

	MsgPostEntityTransformUpdated post_update_msg;
	post_update_msg.mEntity = this;
	post_update_msg.mOldTransform = mTransform;

	MsgPreEntityTransformUpdated pre_update_msg;
	pre_update_msg.mEntity = this;
	pre_update_msg.mNewTransform = inTransform;
	SendMessage(pre_update_msg);

	mTransform = pre_update_msg.mNewTransform;

	post_update_msg.mNewTransform = mTransform;

	SendMessage(post_update_msg);
}

void Entity::SetPosition(const fm::vec2& inPosition)
{
	fm::Transform2D new_transform = GetTransform();
	new_transform.position = inPosition;
	SetTransform(new_transform);
}

void Entity::SetHalfSize(const fm::vec2& inHalfSize)
{
	fm::Transform2D new_transform = GetTransform();
	new_transform.halfSize = inHalfSize;
	SetTransform(new_transform);
}

void Entity::SetRotation(float inRotation)
{
	fm::Transform2D new_transform = GetTransform();
	new_transform.rotation = inRotation;
	SetTransform(new_transform);
}

const fm::Transform2D& Entity::GetTransform() const
{
	gAssert(gIsMainThread());
	return mTransform;
}

fm::vec2 Entity::GetPosition() const
{
	return GetTransform().position;
}

fm::vec2 Entity::GetHalfSize() const
{
	return GetTransform().halfSize;
}

float Entity::GetRotation() const
{
	return GetTransform().rotation;
}
