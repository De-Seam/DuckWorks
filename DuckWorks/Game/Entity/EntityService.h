#pragma once
#include <DuckCore/Containers/Array.h>
#include <DuckCore/Containers/Pair.h>
#include <DuckCore/Containers/UniquePtr.h>
#include <DuckCore/Utilities/Utilities.h>

#include <Game/World/Service.h>

class WorldUpdateHandle;
class EntitySystemTick;
class EntitySystemUpdate;
class Entity;
class EntitySystem;

class EntityService : public Service
{
	RTTI_CLASS(EntityService, Service)
public:
	static constexpr float cTickFrequency = 1.0f / 60.0f;

	EntityService(World& inWorld);
	virtual ~EntityService() override;

	template<typename taType>
	taType* CreateEntitySystem();

	template<typename taType>
	void RegisterEntityType();

	void AddEntity(const DC::Ref<Entity>& inEntity);
	void RemoveEntity(const DC::Ref<Entity>& inEntity);

	const DC::Array<DC::Ref<Entity>>& GetEntities() const { return mEntities; }

	void Update(float inDeltaTime);

private:
	void AddEntitySystem(DC::UniquePtr<EntitySystem> inEntitySystem);

	DC::Ref<WorldUpdateHandle> mWorldUpdateHandle;

	DC::Array<DC::Pair<DC::RTTI*, std::function<Entity*(World&)>>> mEntityTypesAndConstructors;

	DC::Array<DC::Ref<Entity>> mEntities;

	DC::Array<DC::UniquePtr<EntitySystemUpdate>> mEntitySystemsToUpdate;
	DC::Array<DC::UniquePtr<EntitySystemTick>> mEntitySystemsToTick;
	float mTimeSinceTick = 0.0f;
};

template<typename taType>
taType* EntityService::CreateEntitySystem()
{
	DC::UniquePtr<taType> entity_system = DC::gMakeUnique<taType>(GetWorld());
	taType* entity_system_ptr = entity_system;
	AddEntitySystem(DC::UniquePtr<EntitySystem>(DC::gMove(entity_system)));
	return entity_system_ptr;
}

template <typename taType>
void EntityService::RegisterEntityType()
{
	DC::RTTI* rtti = &taType::sGetRTTI();
	gAssert(
	mEntityTypesAndConstructors.FindIf([rtti](const DC::Pair<DC::RTTI*, std::function<Entity*(World&)>>& pair)
	{
		return pair.mFirst == rtti;
	}) == -1, "Entity type was already registered.");

	mEntityTypesAndConstructors.Add(&taType::sGetRTTI(), [](World& inWorld) { return new taType(inWorld); });
}
