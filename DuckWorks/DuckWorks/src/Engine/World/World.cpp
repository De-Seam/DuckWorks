#include "Precomp.h"
#include "Engine/World/World.h"

// Engine includes
#include "Engine/Engine/Engine.h"
#include "Engine/Entity/Components.h"
#include "Engine/Entity/EntityMessages.h"
#include "Engine/Entity/Components/EntityComponentManager.h"
#include "Engine/Entity/Components/ScriptComponent.h"
#include "Engine/Factory/Factory.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Resources/ResourceManager.h"
#include "Engine/Threads/ThreadManager.h"

// External includes
#include <External/SDL/SDL.h>

Json World::Serialize()
{
	PROFILE_SCOPE(World::Serialize)
	gAssert(gIsMainThread());

	Json json = Base::Serialize();

	json.update(SerializeIgnoreEntities());

	for (const Ref<Entity>& entity : mEntities)
		json["Entities"].emplace_back(entity);
	return json;
}

void World::Deserialize(const Json& inJson)
{
	PROFILE_SCOPE(World::Deserialize)
	gAssert(gIsMainThread());

	DeserializeIgnoreEntities(inJson);

	mEntities.clear();

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

	JSON_SAVE(json, mTickRate);

	return json;
}

void World::DeserializeIgnoreEntities(const Json& inJson)
{
	JSON_LOAD(inJson, mTickRate);
	mTickRate = gClamp(mTickRate, 0.005f, 1.0f);
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
		entity->EndPlay();

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

	UpdateObjects(inDeltaTime);

	DestroyEntities();
}

void World::Render()
{
	PROFILE_SCOPE(World::Render)
	gAssert(gIsMainThread());

	static Array<Renderer::DrawTextureParams> sDrawTextureParams;
	sDrawTextureParams.clear();
	gEntityComponentManager.LoopOverComponents<TextureRenderComponent>([](const TextureRenderComponent& inTextureRenderComponent)
	{
		Transform2D transform = inTextureRenderComponent.GetWorldTransform();
		Renderer::DrawTextureParams params;
		params.mTexture = inTextureRenderComponent.mTexture->mTexture;
		params.mPosition = transform.mPosition;
		params.mHalfSize = transform.mHalfSize;
		params.mRotation = transform.mRotation;
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
	gAssert(!HasBegunPlay());

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
	gAssert(HasBegunPlay());

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

TextureRenderComponent* World::GetTextureRenderComponentAtLocationSlow(Vec2 inWorldLocation) const
{
	gAssert(gIsMainThread());
	for (const Ref<Entity> entity : mEntities)
	{
		TextureRenderComponent* texture_render_component = nullptr;
		entity->LoopOverComponents<TextureRenderComponent>([&entity, &inWorldLocation, &texture_render_component](TextureRenderComponent& inTextureRenderComponent)
		{
			const Transform2D& transform = inTextureRenderComponent.GetWorldTransform();

			const Vec2& position = transform.mPosition;
			const Vec2& half_size = transform.mHalfSize;
			const float& rotation = transform.mRotation;

			// Convert the rotation to a normalized direction vector
			Vec2 rotation_dir(cos(rotation), sin(rotation));

			// Translate the world location to the rectangle's local space
			Vec2 local_point = inWorldLocation - position;

			// Rotate the point in the opposite direction of the rectangle's rotation
			// to align it with the rectangle's local axes
			Vec2 rotatedPoint(local_point.mX * rotation_dir.mX + local_point.mY * rotation_dir.mY,
							-local_point.mX * rotation_dir.mY + local_point.mY * rotation_dir.mX);

			// Check if the rotated point lies within the rectangle's bounds
			if (std::abs(rotatedPoint.mX) <= half_size.mX && std::abs(rotatedPoint.mY) <= half_size.mY)
				texture_render_component = &inTextureRenderComponent;
		});

		if (texture_render_component != nullptr)
			return texture_render_component;
	}
	return nullptr;
}

void World::OnEntityUpdateFrequencyChanged(const MsgEntityUpdateFrequencyChanged& inMsg)
{
	UnregisterEntityForUpdates(*inMsg.GetEntity(), inMsg.GetOldUpdateFrequency());
	RegisterEntityForUpdates(*inMsg.GetEntity(), inMsg.GetNewUpdateFrequency());
}

void World::UnregisterEntityForUpdates(Entity& inEntity, Entity::EUpdateFrequency inOldUpdateFrequency)
{
	PROFILE_SCOPE(World::UnregisterEntityForUpdates)
	gAssert(gIsMainThread());

	if (inOldUpdateFrequency == Entity::EUpdateFrequency::None)
		return;

	auto it = std::ranges::find(mEntitiesToUpdate[SCast<uint64>(inOldUpdateFrequency)], &inEntity);
	if (it != mEntitiesToUpdate[SCast<uint64>(inOldUpdateFrequency)].end())
		mEntitiesToUpdate[SCast<uint64>(inOldUpdateFrequency)].erase(it);
}

void World::RegisterEntityForUpdates(Entity& inEntity, Entity::EUpdateFrequency inUpdateFrequency)
{
	PROFILE_SCOPE(World::RegisterEntityForUpdates)
	gAssert(gIsMainThread());

	if (inUpdateFrequency == Entity::EUpdateFrequency::None)
		return;

	mEntitiesToUpdate[SCast<uint64>(inUpdateFrequency)].emplace_back(&inEntity);
}

void World::UpdateObjects(float inDeltaTime)
{
	PROFILE_SCOPE(World::UpdateObjects)
	gAssert(gIsMainThread());

	mUpdateState = EUpdateState::Updating;

	mTimeSinceFullSecond += inDeltaTime;
	mTimeSinceTick += inDeltaTime;

	UpdateFrameEntities(inDeltaTime);

	while (mTimeSinceTick > mTickRate)
	{
		UpdateTickEntities();
		mTimeSinceTick -= mTickRate;
	}

	while (mTimeSinceFullSecond > 1.0f)
	{
		UpdateSecondEntities();
		mTimeSinceFullSecond -= 1.0f;
	}

	UpdateComponents(inDeltaTime);

	mUpdateState = EUpdateState::PreUpdate;
}

void World::UpdateFrameEntities(float inDeltaTime)
{
	PROFILE_SCOPE(World::UpdateFrameEntities)

	for (Entity* entity : mEntitiesToUpdate[SCast<uint64>(Entity::EUpdateFrequency::Frame)])
		entity->Update(inDeltaTime);
}

void World::UpdateTickEntities()
{
	PROFILE_SCOPE(World::UpdateTickEntities)

	for (Entity* entity : mEntitiesToUpdate[SCast<uint64>(Entity::EUpdateFrequency::Tick)])
		entity->Update(mTickRate);
}

void World::UpdateSecondEntities()
{
	PROFILE_SCOPE(World::UpdateSecondEntities)

	for (Entity* entity : mEntitiesToUpdate[SCast<uint64>(Entity::EUpdateFrequency::Second)])
		entity->Update(1.0f);
}

void World::UpdateComponents(float inDeltaTime)
{
	PROFILE_SCOPE(World::UpdateComponents)
	gAssert(gIsMainThread());

	{
		PROFILE_SCOPE(World::UpdateComponents::ScriptComponent)
		gEntityComponentManager.UpdateComponents<ScriptComponent>(inDeltaTime);
	}
	{
		PROFILE_SCOPE(World::UpdateComponents::AnimationComponent)
		gEntityComponentManager.UpdateComponents<AnimationComponent>(inDeltaTime);
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
