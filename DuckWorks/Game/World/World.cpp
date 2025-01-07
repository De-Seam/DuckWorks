#include <Game/World/World.h>

#include <Engine/Engine.h>
#include <Engine/Renderer/TextureResource.h>
#include <Engine/Resources/ResourceManager.h>

#include <Game/Entity/Entity.h>
#include <Game/Entity/EntityService.h>
#include <Game/Entity/Components/GameplayComponents.h>
#include <Game/Entity/Components/RenderComponent.h>
#include <Game/Entity/Systems/RenderSystem.h>
#include <Game/Grid/GridService.h>

using namespace DC;

WorldTickHandle::~WorldTickHandle()
{
	gAssert(mWorld != nullptr);
	mWorld->UnregisterTickCallback(*this);
}

WorldUpdateHandle::~WorldUpdateHandle()
{
	gAssert(mWorld != nullptr);
	mWorld->UnregisterUpdateCallback(*this);
}

World::World()
{
	RegisterComponent<TransformComponent>();
	RegisterComponent<SpriteRenderComponent>();
	RegisterComponent<VelocityComponent>();
	RegisterComponent<MovementComponent>();
	RegisterComponent<BlockingComponent>();

	CreateService<EntityService>();
	CreateService<GridService>();

	//Entity* entity = new Entity(*this);
	//DC::Ref<TextureResource> texture_resource = gEngine->GetManager<ResourceManager>().Get<TextureResource>("Assets/Textures/top.jpg");
	//SpriteRenderComponent& sprite_render_component = entity->AddComponent<SpriteRenderComponent>();
	//sprite_render_component.mSprite = new Sprite(*texture_resource, {0, 0}, {980, 575});
	//TransformComponent& transform_component = entity->AddComponent<TransformComponent>();
	//transform_component.mTransform.mPosition = {0.0f, 0.0f};
	//transform_component.mTransform.mHalfSize = { 256.0f, 256.0f };
	//transform_component.mTransform.mRotation = 0.0f;
	//entity_service.AddEntity(entity);
}

World::~World()
{
	mServices.Clear();
	mRegistry.clear();
}

void World::Update(float inDeltaTime)
{
	for (float& time_since_update : mHzToTimeSinceUpdate)
		time_since_update += inDeltaTime;

	for (int i = 0; i < mTickFrequencyToCallbacks.Length(); i++)
	{
		float& time_since_update = mHzToTimeSinceUpdate[i];
		float target_seconds = mHzToTargetSeconds[i];

		while (time_since_update >= target_seconds)
		{
			time_since_update -= target_seconds;

			Array<TickCallback>& tick_callbacks = mTickFrequencyToCallbacks[i];

			if (tick_callbacks.IsEmpty())
				continue;

			for (const TickCallback& callback : tick_callbacks)
				callback.mCallback(target_seconds);
		}
	}

	for (const UpdateCallback& callback : mUpdateCallbacks)
		callback.mCallback(inDeltaTime);
}

Ref<WorldTickHandle> World::RegisterTickCallback(EWorldTickFrequency inTickFrequency, std::function<void(float)> inCallback)
{
	int next_id = mLastTickHandleID++;
	gAssert(next_id >= 0, "IDs ran out");

	Ref<WorldTickHandle> handle = new WorldTickHandle(next_id, *this, inTickFrequency);
	mTickFrequencyToCallbacks[gStaticCast<int>(inTickFrequency)].Emplace(next_id, inCallback);

	return handle;
}

Ref<WorldUpdateHandle> World::RegisterUpdateCallback(std::function<void(float)> inCallback)
{
	int next_id = mLastUpdateHandleID++;
	gAssert(next_id >= 0, "IDs ran out");

	Ref<WorldUpdateHandle> handle = new WorldUpdateHandle(next_id, *this);
	mUpdateCallbacks.Emplace(next_id, inCallback);

	return handle;
}

void World::AddComponent(entt::entity inEntity, uint64 inComponentTypeIDHashCode)
{
	ComponentData* component_data = mComponentTypeToData.Find(inComponentTypeIDHashCode);
	gAssert(component_data != nullptr);
	component_data->mAddComponentFunc(mRegistry, inEntity);
}

void World::RemoveComponent(entt::entity inEntity, uint64 inComponentTypeIDHashCode)
{
	ComponentData* component_data = mComponentTypeToData.Find(inComponentTypeIDHashCode);
	gAssert(component_data != nullptr);
	component_data->mRemoveComponentFunc(mRegistry, inEntity);
}

bool World::HasComponent(entt::entity inEntity, uint64 inComponentTypeIDHashCode)
{
	ComponentData* component_data = mComponentTypeToData.Find(inComponentTypeIDHashCode);
	gAssert(component_data != nullptr);
	return component_data->mHasComponentFunc(mRegistry, inEntity);
}

const String& World::GetComponentName(uint64 inComponentTypeIDHashCode) const
{
	const ComponentData& component_data = mComponentTypeToData.At(inComponentTypeIDHashCode);
	return component_data.mName;
}

void World::GetComponentNames(Array<String>& outComponentNames) const
{
	gAssert(outComponentNames.IsEmpty());
	mComponentTypeToData.ForEach([&outComponentNames](const uint64& inKey, const ComponentData& inValue)
	{
		outComponentNames.Emplace(inValue.mName);
	});
}

void World::GetComponentNamesAndTypeIDHashCodes(Array<Pair<String, uint64>>& outComponentNamesAndTypeIDHashCodes) const
{
	gAssert(outComponentNamesAndTypeIDHashCodes.IsEmpty());
	mComponentTypeToData.ForEach([&outComponentNamesAndTypeIDHashCodes](const uint64& inKey, const ComponentData& inValue)
	{
		outComponentNamesAndTypeIDHashCodes.Emplace(inValue.mName, inKey);
	});
}

void World::UnregisterTickCallback(const WorldTickHandle& inHandle)
{
	Array<TickCallback>& callbacks = mTickFrequencyToCallbacks[gStaticCast<int>(inHandle.mTickFrequency)];
	const int index = callbacks.FindIf([&](const TickCallback& inItem) { return inItem.mID == inHandle.GetID(); });
	gAssert(index != -1);
	callbacks.SwapRemove(index);
}

void World::UnregisterUpdateCallback(const WorldUpdateHandle& inHandle)
{
	const int index = mUpdateCallbacks.FindIf([&](const UpdateCallback& inItem) { return inItem.mID == inHandle.GetID(); });
	gAssert(index != -1);
	mUpdateCallbacks.SwapRemove(index);
}
