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

Json World::Serialize() const
{
	PROFILE_SCOPE(World::Serialize)

	Json json = Base::Serialize();

	JSON_SAVE(json, mVelocityIterations);
	JSON_SAVE(json, mPositionIterations);
	JSON_SAVE(json, mPhysicsUpdateFrequency);
	JSON_SAVE(json, mPhysicsTimeStep);

	for (const EntityPtr& entity : mEntities)
	{
		json["Entities"].push_back(entity->Serialize());
	}
	return json;
}

void World::Deserialize(const Json& inJson)
{
	PROFILE_SCOPE(World::Deserialize)

	Base::Deserialize(inJson);

	JSON_TRY_LOAD(inJson, mVelocityIterations);
	JSON_TRY_LOAD(inJson, mPositionIterations);
	JSON_TRY_LOAD(inJson, mPhysicsUpdateFrequency);
	JSON_TRY_LOAD(inJson, mPhysicsTimeStep);

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

World::World()
{
	mPhysicsWorld = std::make_unique<b2World>(b2Vec2(0.0f, 98.f));
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

	ScopedMutexReadLock lock{mEntitiesMutex};

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
	inEntity->AddComponent<EntityComponent>(inEntity);
	inEntity->AddComponent<NameComponent>(inName);
	mEntities.push_back(inEntity);
	inEntity->BeginPlay();
	return mEntities.back();
}

b2Body* World::CreatePhysicsBody(const b2BodyDef& inBodyDef, const Array<b2FixtureDef>& inFixtureDefs)
{
	b2Body* body = CreatePhysicsBody(inBodyDef);
	for (const b2FixtureDef& fixture_def : inFixtureDefs)
		body->CreateFixture(&fixture_def);
	return body;
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

void World::DestroyPhysicsBody(b2Body* inBody)
{
	ScopedMutexWriteLock lock{mPhysicsWorldMutex};
	mPhysicsWorld->DestroyBody(inBody);
}

EntityPtr World::GetEntityAtLocationSlow(fm::vec2 inWorldLocation)
{
	ScopedMutexReadLock lock{mEntitiesMutex};

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
			return base_entity.GetComponent<EntityComponent>().mEntity.lock(); // Assuming EntityPtr can be constructed from entity directly
		}
	}
	return nullptr;
}

void World::UpdatePhysics(float inDeltaTime)
{
	PROFILE_SCOPE(World::UpdatePhysics)


	{
		auto view = mRegistry.view<TransformComponent, PhysicsComponent, PhysicsPositionOrRotationUpdatedTag>();
		{
			ScopedMutexReadLock lock{mEntitiesMutex};
			for (auto entity : view)
			{
				fm::Transform2D& transform = view.get<TransformComponent>(entity).mTransform;
				PhysicsComponent& physics = view.get<PhysicsComponent>(entity);

				// Make sure the body is valid
				gAssert(physics.mBody != nullptr, "PhysicsComponent has no body");

				fm::vec2 position = transform.position + physics.mOffset;

				physics.mBody->SetTransform(b2Vec2{position.x, position.y}, transform.rotation);
			}
		}
		for (auto entity : view)
		{
			BaseEntity entity_handler = {entity, this};
			entity_handler.RemoveComponent<PhysicsPositionOrRotationUpdatedTag>();
		}
	}
	{
		auto view = mRegistry.view<PhysicsComponent, PhysicsSizeUpdatedTag>();
		{
			for (auto entity : view)
			{
				PhysicsComponent& physics = view.get<PhysicsComponent>(entity);

				// Make sure the body is valid
				gAssert(physics.mBody != nullptr, "PhysicsComponent has no body");

				b2Shape* shape = physics.mBody->GetFixtureList()->GetShape();
				switch (shape->GetType())
				{
				case b2Shape::e_polygon:
				{
					gChangeB2BoxSize(physics.mBody, physics.mHalfSize);
				}
				break;
				default:
					shape->m_radius = physics.mHalfSize.x;
					break;
				}
			}
			ScopedMutexReadLock lock{mEntitiesMutex};
		}
		for (auto entity : view)
		{
			BaseEntity entity_handler = {entity, this};
			entity_handler.RemoveComponent<PhysicsSizeUpdatedTag>();
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
			gAssert(physics.mBody != nullptr, "PhysicsComponent has no body");

			transform.position = fm::vec2{physics.mBody->GetPosition().x, physics.mBody->GetPosition().y} - physics.mOffset;
			transform.rotation = physics.mBody->GetAngle();
		}
	}
}

void World::PhysicsTimeStep()
{
	PROFILE_SCOPE(World::PhysicsTimeStep)

	ScopedMutexWriteLock lock{mPhysicsWorldMutex};

	//Fixed time step for physics world
	mPhysicsWorld->Step(mPhysicsTimeStep, mVelocityIterations, mPositionIterations);
}
