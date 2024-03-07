#include "Precomp.h"
#include "Engine/World/World.h"

// Engine includes
#include "Engine/Entity/Actor.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Resources/ResourceManager.h"

// External includes
#include <External/box2d/box2d.h>
#include <External/SDL/SDL.h>

// Game includes (ILLEGAL!)
#include "Engine/Factory/Factory.h"
#include "Engine/Renderer/AnimationManager.h"
#include "Game/App/App.h"
#include "Game/Entity/Player/Player.h"


World::World()
{
	mPhysicsWorld = std::make_unique<b2World>(b2Vec2(0.0f, 0.0f));
}

Json World::Serialize()
{
	PROFILE_SCOPE(World::Serialize)

	Json json;

	JSON_SAVE(json, mVelocityIterations);
	JSON_SAVE(json, mPositionIterations);
	JSON_SAVE(json, mPhysicsUpdateFrequency);
	JSON_SAVE(json, mPhysicsTimeStep);
	JSON_SAVE(json, mPhysicsTimeAccumulator);

	for (EntityPtr& entity : mEntities)
	{
		json["Entities"].push_back(entity->Serialize());
	}
	return json;
}

void World::Deserialize(const Json& inJson)
{
	PROFILE_SCOPE(World::Deserialize)

	JSON_TRY_LOAD(inJson, mVelocityIterations);
	JSON_TRY_LOAD(inJson, mPositionIterations);
	JSON_TRY_LOAD(inJson, mPhysicsUpdateFrequency);
	JSON_TRY_LOAD(inJson, mPhysicsTimeStep);
	JSON_TRY_LOAD(inJson, mPhysicsTimeAccumulator);

	if (inJson.contains("Entities"))
	{
		for (const Json& json_entity : inJson["Entities"])
		{
			String class_name = json_entity.begin().key();
			EntityPtr entity = gEntityFactory.CreateClass(class_name, this);

			const Json& components = json_entity[class_name]["Components"];

			String name = "Empty";
			if (components.contains("NameComponent"))
				name = components["NameComponent"]["mName"];

			AddEntity(entity, name);

			entity->Deserialize(json_entity[class_name]);
		}
	}
}

void World::Update(float inDeltaTime)
{
	PROFILE_SCOPE(World::Update)

	UpdateEntities(inDeltaTime);

	UpdatePhysics(inDeltaTime);

	DestroyEntities();
}

void World::Render(float inDeltaTime)
{
	PROFILE_SCOPE(World::Render)

	if (!gApp.IsPaused())
	{
		gAnimationManager.Update(this, inDeltaTime);
	}

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

void World::UpdateEntities(float inDeltaTime)
{
	ScopedMutexReadLock lock{mEntitiesMutex};

	for (std::shared_ptr<Entity>& entity : mEntities)
	{
		entity->Update(inDeltaTime);
	}
}

void World::DestroyEntities()
{
	PROFILE_SCOPE(World::DestroyEntities)

	ScopedMutexWriteLock lock{mEntitiesMutex};

	auto view = mRegistry.view<DestroyedTag>();
	for (entt::entity entity_handle : view)
	{
		DestroyedTag& destroyed_tag = view.get<DestroyedTag>(entity_handle);

		for (auto rit = mEntities.rbegin(); rit != mEntities.rend(); ++rit)
		{
			SharedPtr<Entity>& entity = *rit;
			if (entity->mUID == destroyed_tag.mUID)
			{
				gAssert(entity->GetEntityHandle() == entity_handle, "This could indicate multiple entities with the same UID somehow");

				entity->EndPlay();
				entity->RemoveComponent<DestroyedTag>();

				std::iter_swap(rit, mEntities.rbegin());
				mEntities.pop_back();
				break;
			}
		}
	}
}

EntityPtr World::AddEntity(const EntityPtr& inEntity, const String& inName)
{
	PROFILE_SCOPE(World::AddEntity)
	ScopedMutexWriteLock lock{mEntitiesMutex};

	inEntity->AddComponent<NameComponent>(inName);
	mEntities.push_back(inEntity);
	inEntity->BeginPlay();
	return mEntities.back();
}

b2Body* World::CreatePhysicsBody(const b2BodyDef& inBodyDef, const b2FixtureDef& inFixtureDef)
{
	b2Body* body = CreatePhysicsBody(inBodyDef);
	body->CreateFixture(&inFixtureDef);
	return body;
}

b2Body* World::CreatePhysicsBody(const b2BodyDef& inBodyDef)
{
	ScopedMutexWriteLock lock{mPhysicsWorldMutex};
	return mPhysicsWorld->CreateBody(&inBodyDef);
}

void World::UpdatePhysics(float inDeltaTime)
{
	PROFILE_SCOPE(World::UpdatePhysics);

	{
		auto view = mRegistry.view<TransformComponent, PhysicsComponent, PhysicsPositionUpdatedTag>();
		for (auto entity : view)
		{
			fm::Transform2D& transform = view.get<TransformComponent>(entity).mTransform;
			PhysicsComponent& physics = view.get<PhysicsComponent>(entity);

			// Make sure the body is valid
			gDebugIf(physics.mBody == nullptr)
				return;

			physics.mBody->SetTransform({transform.position.x, transform.position.y}, transform.rotation);

			BaseEntity entity_handler = {entity, this};
			entity_handler.RemoveComponent<PhysicsPositionUpdatedTag>();
		}
	}

	mPhysicsTimeAccumulator += inDeltaTime;
	while (mPhysicsTimeAccumulator >= mPhysicsTimeStep)
	{
		PhysicsTimeStep();
		mPhysicsTimeAccumulator -= mPhysicsTimeStep;
	}

	{
		auto view = mRegistry.view<TransformComponent, PhysicsComponent>();
		for (auto entity : view)
		{
			fm::Transform2D& transform = view.get<TransformComponent>(entity).mTransform;
			PhysicsComponent& physics = view.get<PhysicsComponent>(entity);

			// Make sure the body is valid
			gDebugIf(physics.mBody == nullptr)
				return;

			transform.position = {physics.mBody->GetPosition().x, physics.mBody->GetPosition().y};
			transform.rotation = physics.mBody->GetAngle();
		}
	}
}

void World::PhysicsTimeStep()
{
	PROFILE_SCOPE(World::PhysicsTimeStep)

	//Fixed time step for physics world
	mPhysicsWorld->Step(mPhysicsTimeStep, mVelocityIterations, mPositionIterations);
}
