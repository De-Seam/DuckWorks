#include "Precomp.h"
#include "Engine/World/World.h"

// Engine includes
#include "Engine/Entity/Actor.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Resources/ResourceManager.h"

// External includes
#include <External/SDL/SDL.h>

// Game includes (ILLEGAL!)
#include "Engine/Engine/Engine.h"
#include "Engine/Entity/Components.h"
#include "Engine/Entity/Components/ScriptComponent.h"
#include "Engine/Factory/Factory.h"
#include "Engine/Threads/ThreadManager.h"

RTTI_CLASS_DEFINITION(World, StandardAllocator)

Json World::Serialize()
{
	PROFILE_SCOPE(World::Serialize)
	gAssert(gIsMainThread());

	Json json = Base::Serialize();

	json.update(SerializeIgnoreEntities());

	for (const Ref<Entity>& entity : mEntities)
	{
		Json entity_json;
		entity_json = entity->Serialize();
		json["Entities"].emplace_back(entity_json);
	}
	return json;
}

void World::Deserialize(const Json& inJson)
{
	PROFILE_SCOPE(World::Deserialize)
	gAssert(gIsMainThread());

	mEntities.clear();

	JSON_TRY_LOAD(inJson, mVelocityIterations);
	JSON_TRY_LOAD(inJson, mPositionIterations);
	JSON_TRY_LOAD(inJson, mPhysicsUpdateFrequency);
	JSON_TRY_LOAD(inJson, mPhysicsTimeStep);

	if (inJson.contains("Entities"))
	{
		for (const Json& json_entity : inJson["Entities"])
		{
			if (!json_entity.contains("ClassName"))
				continue;

			String class_name = json_entity["ClassName"];
			Entity::ConstructParameters params;
			params.mWorld = this;
			params.mName = "Empty";
			Ref<Entity> entity = gEntityFactory.CreateClass(class_name, params);
			entity->Deserialize(json_entity);

			AddEntity(entity);
		}
	}
}

Json World::SerializeIgnoreEntities() const
{
	gAssert(gIsMainThread());

	Json json;

	JSON_SAVE(json, mVelocityIterations);
	JSON_SAVE(json, mPositionIterations);
	JSON_SAVE(json, mPhysicsUpdateFrequency);
	JSON_SAVE(json, mPhysicsTimeStep);

	return json;
}

World::World(const ConstructParameters& inConstructParameters)
	: Base(inConstructParameters)
{
	PROFILE_SCOPE(World::World)
	gLog(ELogType::Info, "Constructing World");

	mCollisionWorld = std::make_unique<CollisionWorld>();
}

World::~World()
{
	PROFILE_SCOPE(World::~World)
	gLog(ELogType::Info, "Destroying World");

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
	gAssert(gIsMainThread());

	AddEntities();

	UpdateEntities(inDeltaTime);

	UpdateComponents(inDeltaTime);

	DestroyEntities();
}

void World::Render(float inDeltaTime)
{
	PROFILE_SCOPE(World::Render)
	gAssert(gIsMainThread());

	if (!gEngine.IsPaused())
	{
		gEntityComponentManager.UpdateComponents<AnimationComponent>(inDeltaTime);
	}

	static Array<Renderer::DrawTextureParams> sDrawTextureParams;
	sDrawTextureParams.clear();
	gEntityComponentManager.LoopOverComponents<TextureRenderComponent>([](const TextureRenderComponent& inTextureRenderComponent)
	{
		fm::Transform2D transform = inTextureRenderComponent.GetEntity()->GetTransform();
		Renderer::DrawTextureParams params;
		params.mTexture = inTextureRenderComponent.mTexture->mTexture;
		params.mPosition = transform.position;
		params.mHalfSize = transform.halfSize;
		params.mRotation = transform.rotation;
		params.mFlip = inTextureRenderComponent.mFlip;
		if (inTextureRenderComponent.mUseSrcRect)
			params.mSrcRect = inTextureRenderComponent.mSrcRect;
		sDrawTextureParams.emplace_back(params);
	});
	gRenderer.DrawTextures(sDrawTextureParams);
}

void World::BeginPlay()
{
	PROFILE_SCOPE(World::BeginPlay)
	gLog(ELogType::Info, "World Beginning Play");

	gAssert(gIsMainThread());

	mState = EWorldState::BeginningPlay;

	mCollisionWorld->BeginPlay();

	for (Ref<Entity>& entity : mEntities)
		entity->BeginPlay();

	mState = EWorldState::Playing;
}

void World::EndPlay()
{
	PROFILE_SCOPE(World::EndPlay)
	gLog(ELogType::Info, "World Ending Play");

	gAssert(gIsMainThread());

	mState = EWorldState::EndingPlay;

	for (Ref<Entity>& entity : mEntities)
		entity->EndPlay();

	mState = EWorldState::EndedPlay;
}

void World::AddEntity(const Ref<Entity>& inEntity)
{
	PROFILE_SCOPE(World::AddEntity)
	inEntity->Init();

	if (mUpdateState == EUpdateState::Updating)
	{
		ScopedUniqueMutexLock lock(mEntitiesToAddMutex);
		mEntitiesToAdd.push_back(inEntity);
		return;
	}

	gAssert(gIsMainThread());
	mEntities.push_back(inEntity);
}

void World::DestroyEntity(const Ref<Entity>& inEntity)
{
	PROFILE_SCOPE(World::DestroyEntity)
	if (mUpdateState == EUpdateState::Updating)
	{
		ScopedUniqueMutexLock lock(mEntitiesToRemoveMutex);
		mEntitiesToRemove.push_back(inEntity);
		return;
	}

	gAssert(gIsMainThread());
	inEntity->EndPlay();
	std::erase(mEntities, inEntity);
}

Optional<Ref<Entity>> World::GetEntityAtLocationSlow(fm::vec2 inWorldLocation)
{
	gAssert(gIsMainThread());
	for (const Ref<Entity> entity : mEntities)
	{
		fm::Transform2D transform = entity->GetTransform();
		fm::vec2& position = transform.position;
		fm::vec2& half_size = transform.halfSize;
		float& rotation = transform.rotation;

		// Convert the rotation to a normalized direction vector
		fm::vec2 rotation_dir(cos(rotation), sin(rotation));

		// Translate the world location to the rectangle's local space
		fm::vec2 local_point = inWorldLocation - position;

		// Rotate the point in the opposite direction of the rectangle's rotation
		// to align it with the rectangle's local axes
		fm::vec2 rotatedPoint(local_point.x * rotation_dir.x + local_point.y * rotation_dir.y,
							-local_point.x * rotation_dir.y + local_point.y * rotation_dir.x);

		// Check if the rotated point lies within the rectangle's bounds
		if (std::abs(rotatedPoint.x) <= half_size.x && std::abs(rotatedPoint.y) <= half_size.y)
		{
			return entity;
		}
	}
	return NullOpt;
}

void World::UpdateEntities(float inDeltaTime)
{
	PROFILE_SCOPE(World::UpdateEntities)
	gAssert(gIsMainThread());

	mUpdateState = EUpdateState::Updating;
	for (Ref<Entity>& entity : mEntities)
		entity->Update(inDeltaTime);
	mUpdateState = EUpdateState::PreUpdate;
}

void World::UpdateComponents(float inDeltaTime)
{
	PROFILE_SCOPE(World::UpdateComponents)
	gAssert(gIsMainThread());

	{
		PROFILE_SCOPE(World::UpdateComponents::ScriptComponent)

		gEntityComponentManager.LoopOverComponents<ScriptComponent>([inDeltaTime](ScriptComponent& inScriptComponent)
		{
			inScriptComponent.Update(inDeltaTime);
		});
	}
}

void World::AddEntities()
{
	PROFILE_SCOPE(World::AddEntities)
	gAssert(gIsMainThread());

	if (mEntitiesToAdd.empty())
		return;

	ScopedUniqueMutexLock lock(mEntitiesToAddMutex);
	for (Ref<Entity>& entity : mEntitiesToAdd)
	{
		mEntities.push_back(entity);
		entity->BeginPlay();
	}
	mEntitiesToAdd.clear();
}

void World::DestroyEntities()
{
	PROFILE_SCOPE(World::DestroyEntities)
	gAssert(gIsMainThread());

	if (mEntitiesToRemove.empty())
		return;

	ScopedUniqueMutexLock lock(mEntitiesToRemoveMutex);
	for (Ref<Entity>& entity : mEntitiesToRemove)
	{
		entity->EndPlay();
		std::erase(mEntities, entity);
	}
	mEntitiesToRemove.clear();
}
