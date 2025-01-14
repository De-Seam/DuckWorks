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

struct ComponentBase;
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

	template<typename taType>
	taType& GetService();

	[[nodiscard]]
	DC::Ref<WorldTickHandle> RegisterTickCallback(EWorldTickFrequency inTickFrequency, std::function<void(float)> inCallback);

	[[nodiscard]]
	DC::Ref<WorldUpdateHandle> RegisterUpdateCallback(std::function<void(float)> inCallback);

	entt::registry& GetRegistry() { return mRegistry; }
	const entt::registry& GetRegistry() const { return mRegistry; }

	template<typename taComponent>
	void RegisterComponent();
	void AddComponent(entt::entity inEntity, DC::RTTI& inRTTI);
	void RemoveComponent(entt::entity inEntity, DC::RTTI& inRTTI);
	bool HasComponent(entt::entity inEntity, DC::RTTI& inRTTI);
	ComponentBase* GetComponent(entt::entity inEntity, DC::RTTI& inRTTI);
	void GetComponentNames(DC::Array<DC::String>& outComponentNames) const;
	struct ComponentData
	{
		DC::RTTI* mRTTI = nullptr;
		std::function<void(entt::registry&, entt::entity)> mAddComponentFunc;
		std::function<void(entt::registry&, entt::entity)> mRemoveComponentFunc;
		std::function<bool(const entt::registry&, entt::entity)> mHasComponentFunc;
		std::function<ComponentBase*(entt::registry&, entt::entity)> mGetComponentFunc;
	};
	const DC::HashMap<DC::RTTI*, ComponentData>& GetRTTIToComponentDataMap() const { return mRTTIToComponentData; }

private:
	void UnregisterTickCallback(const WorldTickHandle& inHandle);
	void UnregisterUpdateCallback(const WorldUpdateHandle& inHandle);

	DC::HashMap<DC::RTTITypeID, DC::UniquePtr<Service>> mServices;

	entt::registry mRegistry;
	DC::HashMap<DC::RTTI*, ComponentData> mRTTIToComponentData;

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

template <typename taType>
taType& World::GetService()
{
	const DC::RTTITypeID& type_id = taType::sGetRTTI().GetTypeID();
	return *static_cast<taType*>(mServices[type_id].Get());
}

template <typename taComponent>
void World::RegisterComponent()
{
	DC::RTTI* rtti = &taComponent::sGetRTTI();
	gAssert(!mRTTIToComponentData.Contains(rtti));
	ComponentData& component_data = mRTTIToComponentData[rtti];
	component_data.mRTTI = rtti;
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
	component_data.mGetComponentFunc = [](entt::registry& ioRegistry, entt::entity inEntity)
	{
		return &ioRegistry.get<taComponent>(inEntity);
	};
}
