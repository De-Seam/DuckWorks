#include "Precomp.h"
#include "Engine/World/World.h"

// Engine includes
#include "Engine/Entity/Actor.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Resources/ResourceManager.h"

// External includes
#include <External/SDL/SDL.h>

// Game includes (ILLEGAL!)
#include "Engine/Factory/Factory.h"
#include "Engine/Renderer/AnimationManager.h"
#include "Engine/Threads/ThreadManager.h"

#include "Game/App/App.h"
#include "Game/Entity/Player/Player.h"

RTTI_CLASS_DEFINITION(World)

Json World::Serialize()
{
	PROFILE_SCOPE(World::Serialize)

	Json json = Base::Serialize();

	json.update(SerializeIgnoreEntities());

	for (const Ref<Entity>& entity : mEntities)
	{
		Json entity_json;
		entity_json[entity->GetClassName()] = entity->Serialize();
		json["Entities"].emplace_back(entity_json);
		//json["Entities"].emplace_back(entity->Serialize());
	}
	return json;
}

void World::Deserialize(const Json& inJson)
{
	PROFILE_SCOPE(World::Deserialize)

	ScopedMutexWriteLock lock(mEntitiesMutex);
	mEntities.clear();

	JSON_TRY_LOAD(inJson, mVelocityIterations);
	JSON_TRY_LOAD(inJson, mPositionIterations);
	JSON_TRY_LOAD(inJson, mPhysicsUpdateFrequency);
	JSON_TRY_LOAD(inJson, mPhysicsTimeStep);

	if (inJson.contains("Entities"))
	{
		for (const Json& json_entity : inJson["Entities"])
		{
			String class_name = json_entity.begin().key();
			Ref<Entity> entity = gEntityFactory.CreateClass(class_name);

			const Json& components = json_entity[class_name]["Components"];

			String name = "Empty";
			if (components.contains("NameComponent"))
				name = components["NameComponent"]["mName"];

			AddEntity(entity, name);

			entity->Deserialize(json_entity[class_name]);
		}
	}
}

Json World::SerializeIgnoreEntities() const
{
	Json json;

	JSON_SAVE(json, mVelocityIterations);
	JSON_SAVE(json, mPositionIterations);
	JSON_SAVE(json, mPhysicsUpdateFrequency);
	JSON_SAVE(json, mPhysicsTimeStep);

	return json;
}

World::World()
{
	mCollisionWorld = std::make_unique<CollisionWorld>();
}

World::~World()
{
	for (Ref<Entity>& entity : mEntities)
	{
		entity->EndPlay();
	}

#ifdef _DEBUG
	Array<WeakRef<Entity>> entities;
	entities.reserve(mEntities.size());
	for (Ref<Entity>& entity : mEntities)
		entities.push_back(entity);
#endif
	mEntities.clear();

#ifdef _DEBUG
	for (WeakRef<Entity>& entity : entities)
	{
		gAssert(!entity.IsAlive(), "Entity still has lingering references!");
	}
#endif
}

void World::Update(float inDeltaTime)
{
	PROFILE_SCOPE(World::Update)

	UpdateEntities(inDeltaTime);

	DestroyEntities();
}

void World::Render(float inDeltaTime)
{
	PROFILE_SCOPE(World::Render)

	if (!gApp.IsPaused())
	{
		gAnimationManager.Update(this, inDeltaTime);
	}

	ScopedMutexReadLock texture_render_lock{TextureRenderComponent::sComponentMutex};
	ScopedMutexReadLock transform_lock{TransformComponent::sComponentMutex};

	auto view = mRegistry.view<TextureRenderComponent, TransformComponent>();
	view.each([](const TextureRenderComponent& inRenderComponent, const TransformComponent& inTransformComponent)
	{
		Renderer::DrawTextureParams params;
		params.mTexture = inRenderComponent.mTexture->mTexture;
		params.mPosition = inTransformComponent.mTransform.position;
		params.mHalfSize = inTransformComponent.mTransform.halfSize;
		params.mRotation = inTransformComponent.mTransform.rotation;
		params.mFlip = inRenderComponent.mFlip;
		params.mSrcRect = inRenderComponent.mUseSrcRect ? &inRenderComponent.mSrcRect : nullptr;

		gRenderer.DrawTexture(params);
	});
}

void World::BeginPlay()
{
	PROFILE_SCOPE(World::BeginPlay)
	mBegunPlay = true;
	mCollisionWorld->BeginPlay();

	for (Ref<Entity>& entity : mEntities)
		entity->BeginPlay();
}

void World::EndPlay()
{
	PROFILE_SCOPE(World::EndPlay)
	for (Ref<Entity>& entity : mEntities)
		entity->EndPlay();
}

class UpdateEntityThreadTask : public ThreadTask
{
public:
	virtual void Execute() override
	{
		if (!mEntity.IsAlive())
			return;
		Ref<Entity> entity = mEntity.Get();
		entity->Update(mDeltaTime);
	}

	WeakRef<Entity> mEntity;
	float mDeltaTime;
};

void World::UpdateEntities(float inDeltaTime)
{
	PROFILE_SCOPE(World::UpdateEntities)
	ScopedMutexReadLock lock{mEntitiesMutex};

	static Array<SharedPtr<UpdateEntityThreadTask>> entity_update_tasks;
	for (uint64 i = entity_update_tasks.size(); i < mEntities.size(); i++)
		entity_update_tasks.push_back(std::make_shared<UpdateEntityThreadTask>());

	for (uint64 i = 0; i < mEntities.size(); i++)
	{
		Ref<Entity>& entity = mEntities[i];
		SharedPtr<UpdateEntityThreadTask> task = entity_update_tasks[i];
		task->Reset();
		task->mEntity = entity;
		task->mDeltaTime = inDeltaTime;

		gThreadManager.AddTask(task, ThreadPriority::High);
	}

	for (SharedPtr<UpdateEntityThreadTask>& task : entity_update_tasks)
		task->WaitUntilCompleted();
}

void World::DestroyEntities()
{
	PROFILE_SCOPE(World::DestroyEntities)

	ScopedMutexReadLock destroyed_tag_lock(DestroyedTag::sComponentMutex);
	auto view = mRegistry.view<DestroyedTag>();
	for (entt::entity entity_handle : view)
	{
		DestroyedTag& destroyed_tag = view.get<DestroyedTag>(entity_handle);

		ScopedMutexWriteLock entities_write_lock(mEntitiesMutex);
		for (auto rit = mEntities.rbegin(); rit != mEntities.rend(); ++rit)
		{
			Ref<Entity>& entity = *rit;
			if (entity->mUID == destroyed_tag.mUID)
			{
				gAssert(entity->GetEntityHandle() == entity_handle, "This could indicate multiple entities with the same UID somehow");

				entity->EndPlay();
				// Removing raw here so we don't double lock the mutex.
				mRegistry.remove<DestroyedTag>(entity->GetEntityHandle());

				std::iter_swap(rit, mEntities.rbegin());
				mEntities.pop_back();
				break;
			}
		}
	}
}

Ref<Entity> World::AddEntity(const Ref<Entity>& inEntity, const String& inName, Entity::InitParams inInitParams)
{
	PROFILE_SCOPE(World::AddEntity)
	inInitParams.mWorld = this;
	inEntity->Init(inInitParams);
	inEntity->AddComponent<EntityComponent>(inEntity);
	inEntity->AddComponent<NameComponent>(inName);
	mEntities.push_back(inEntity);
	if (mBegunPlay)
		inEntity->BeginPlay();
	return mEntities.back();
}

Optional<Ref<Entity>> World::GetEntityAtLocationSlow(fm::vec2 inWorldLocation)
{
	ScopedMutexReadLock transform_lock{TransformComponent::sComponentMutex};
	auto view = mRegistry.view<TransformComponent>();
	for (auto entity : view)
	{
		fm::Transform2D& transform = view.get<TransformComponent>(entity).mTransform;
		fm::vec2& position = transform.position;
		fm::vec2& half_size = transform.halfSize;
		float& rotation = transform.rotation;

		// Convert the rotation to a normalized direction vector
		fm::vec2 rotation_dir(cos(rotation), sin(rotation));

		// Translate the world location to the rectangle's local space
		fm::vec2 local_point = inWorldLocation - position;

		// Rotate the point in the opposite direction of the rectangle's rotation
		// to align it with the rectangle's local axes
		fm::vec2 rotatedPoint(
			local_point.x * rotation_dir.x + local_point.y * rotation_dir.y,
			-local_point.x * rotation_dir.y + local_point.y * rotation_dir.x
		);

		// Check if the rotated point lies within the rectangle's bounds
		if (std::abs(rotatedPoint.x) <= half_size.x && std::abs(rotatedPoint.y) <= half_size.y)
		{
			// The point is inside this entity's rotated bounding box
			BaseEntity base_entity = {entity, this};
			return base_entity.GetComponent<EntityComponent>()->mEntity.Get(); // Assuming EntityPtr can be constructed from entity directly
		}
	}
	return NullOpt;
}
