#pragma once
// Core includes
#include <DuckCore/Containers/Array.h>
#include <DuckCore/Containers/Handle.h>
#include <DuckCore/Containers/HashMap.h>
#include <DuckCore/Containers/StaticArray.h>
#include <DuckCore/Containers/UniquePtr.h>

// Engine includes
#include <Engine/Objects/Object.h>

// External includes
#include <DuckCore/Containers/Pair.h>
#include <External/entt/entity/registry.hpp>

class Entity;
class World;

enum class EWorldTickFrequency : uint8
{
	Hz1,
	Hz15,
	Hz30,
	Hz60,

	Num
};

// Update every tick, determined by EWorldTickFrequency
class WorldTickHandle : public DC::Handle
{
public:
	virtual ~WorldTickHandle() override;

private:
	WorldTickHandle(int inID, World& inWorld, EWorldTickFrequency inTickFrequency) :
		Handle(inID),
		mWorld(&inWorld),
		mTickFrequency(inTickFrequency)
	{}

	World* mWorld = nullptr;
	EWorldTickFrequency mTickFrequency = EWorldTickFrequency::Hz1;

	friend class World;
};

// Update every frame
class WorldUpdateHandle : public DC::Handle
{
public:
	virtual ~WorldUpdateHandle() override;

private:
	WorldUpdateHandle(int inID, World& inWorld) :
		Handle(inID),
		mWorld(&inWorld)
	{}

	World* mWorld = nullptr;

	friend class World;
};

class Service;

// World
// Variable: 
// In-game minute: 1 second?
// In-game hour: 60 seconds?
// In-game day: 24 minutes?
// Tick: 60 hz
// Update: every frame
class World : public Object
{
	RTTI_CLASS(World, Object)
public:
	World();
	virtual ~World() override;

	void Update(float inDeltaTime);

	template<typename taType, typename... taArgs>
	taType& CreateService(taArgs&&... inArgs);

	[[nodiscard]]
	DC::Ref<WorldTickHandle> RegisterTickCallback(EWorldTickFrequency inTickFrequency, std::function<void(float)> inCallback);

	[[nodiscard]]
	DC::Ref<WorldUpdateHandle> RegisterUpdateCallback(std::function<void(float)> inCallback);

	entt::registry& GetRegistry() { return mRegistry; }
	const entt::registry& GetRegistry() const { return mRegistry; }

	template<typename taComponent>
	void RegisterComponent();
	void AddComponent(entt::entity inEntity, uint64 inComponentTypeIDHashCode);
	void RemoveComponent(entt::entity inEntity, uint64 inComponentTypeIDHashCode);
	bool HasComponent(entt::entity inEntity, uint64 inComponentTypeIDHashCode);
	const DC::String& GetComponentName(uint64 inComponentTypeIDHashCode) const;
	void GetComponentNames(DC::Array<DC::String>& outComponentNames) const;
	void GetComponentNamesAndTypeIDHashCodes(DC::Array<DC::Pair<DC::String, uint64>>& outComponentNamesAndTypeIDHashCodes) const;

private:
	void UnregisterTickCallback(const WorldTickHandle& inHandle);
	void UnregisterUpdateCallback(const WorldUpdateHandle& inHandle);

	DC::HashMap<DC::RTTITypeID, DC::UniquePtr<Service>> mServices;

	entt::registry mRegistry;
	struct ComponentData
	{
		DC::String mName;
		std::function<void(entt::registry&, entt::entity)> mAddComponentFunc;
		std::function<void(entt::registry&, entt::entity)> mRemoveComponentFunc;
		std::function<bool(const entt::registry&, entt::entity)> mHasComponentFunc;
	};
	DC::HashMap<uint64, ComponentData> mComponentTypeToData;

	struct TickCallback
	{
		int mID = -1;
		std::function<void(float)> mCallback;
	};
	DC::StaticArray<DC::Array<TickCallback>, static_cast<int>(EWorldTickFrequency::Num)> mTickFrequencyToCallbacks;
	int mLastTickHandleID = 0;

	DC::StaticArray<float, static_cast<uint>(EWorldTickFrequency::Num)> mHzToTimeSinceUpdate = { 0.0f, 0.0f, 0.0f, 0.0f };
	DC::StaticArray<float, static_cast<uint>(EWorldTickFrequency::Num)> mHzToTargetSeconds = { 1.0f, 1.0f / 15.0f, 1.0f / 30.0f, 1.0f / 60.0f };

	struct UpdateCallback
	{
		int mID = -1;
		std::function<void(float)> mCallback;
	};
	int mLastUpdateHandleID = 0;
	DC::Array<UpdateCallback> mUpdateCallbacks;

	friend class WorldTickHandle;
	friend class WorldUpdateHandle;
};

template<typename taType, typename ... taArgs>
taType& World::CreateService(taArgs&&... inArgs)
{
	const DC::RTTITypeID& type_id = taType::sGetRTTI().GetTypeID();
	gAssert(!mServices.Contains(type_id), "Service already exists!");

	DC::UniquePtr<taType> service = DC::gMakeUnique<taType>(*this, std::forward<taArgs>(inArgs)...);
	taType* service_ptr = service;
	mServices[type_id] = std::move(service);

	return *service_ptr;
}

template <typename taComponent>
void World::RegisterComponent()
{
	const std::type_info& type_info = typeid(taComponent);
	const uint64 hash_code = type_info.hash_code();
	gAssert(!mComponentTypeToData.Contains(hash_code));
	ComponentData& component_data = mComponentTypeToData[hash_code];
	component_data.mName = type_info.name();
	component_data.mAddComponentFunc = [](entt::registry& ioRegistry, entt::entity inEntity)
	{
		ioRegistry.emplace<taComponent>(inEntity);
	};
	component_data.mRemoveComponentFunc = [](entt::registry& ioRegistry, entt::entity inEntity)
	{
		ioRegistry.remove<taComponent>(inEntity);
	};
	component_data.mHasComponentFunc = [](const entt::registry& inRegistry, entt::entity inEntity)
	{
		return inRegistry.any_of<taComponent>(inEntity);
	};
}
