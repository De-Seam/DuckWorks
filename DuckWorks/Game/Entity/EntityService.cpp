#include <Game/Entity/EntityService.h>

#include <Game/Entity/Entity.h>
#include <Game/Entity/Systems/EntitySystem.h>
#include <Game/Entity/Systems/RenderSystem.h>
#include <Game/World/World.h>

EntityService::EntityService(World& inWorld) :
	Base(inWorld)
{
	CreateEntitySystem<RenderSystem>();

	mWorldUpdateHandle = inWorld.RegisterUpdateCallback([this](float inDeltaTime) { Update(inDeltaTime); });
}

EntityService::~EntityService() = default;

void EntityService::AddEntity(const DC::Ref<Entity>& inEntity)
{
	gAssert(inEntity != nullptr);
	mEntities.Add(inEntity);
}

void EntityService::RemoveEntity(const DC::Ref<Entity>& inEntity)
{
	gAssert(inEntity != nullptr);
	const int index = mEntities.Find(inEntity);
	gAssert(index != -1);
	mEntities.SwapRemove(index);
}

void EntityService::Update(float inDeltaTime)
{
	mTimeSinceTick += inDeltaTime;
	while (mTimeSinceTick >= cTickFrequency)
	{
		for (EntitySystemTick* entity_system_tick : mEntitySystemsToTick)
			entity_system_tick->Tick(cTickFrequency);

		mTimeSinceTick -= inDeltaTime;
	}

	for (EntitySystemUpdate* entity_system_update : mEntitySystemsToUpdate)
		entity_system_update->Update(inDeltaTime);
}

void EntityService::AddEntitySystem(DC::UniquePtr<EntitySystem> inEntitySystem)
{
	if (inEntitySystem->IsA<EntitySystemUpdate>())
		mEntitySystemsToUpdate.Add(gMove(inEntitySystem.StaticCast<EntitySystemUpdate>()));
	else if (inEntitySystem->IsA<EntitySystemTick>())
		mEntitySystemsToTick.Add(gMove(inEntitySystem.StaticCast<EntitySystemTick>()));
#ifdef _ASSERTS
	else
		gAssert(false, "Unknown entity system type");
#endif
}
