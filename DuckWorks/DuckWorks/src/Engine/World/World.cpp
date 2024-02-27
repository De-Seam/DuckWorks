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
#include "Engine/Renderer/AnimationManager.h"
#include "Game/Entity/Player/Player.h"


World::World()
{
	mPhysicsWorld = std::make_unique<b2World>(b2Vec2(0.0f, 0.0f));

	std::shared_ptr<Player> player = CreateEntity<Player>("Player");
	player->SetVelocity({50.f, 0.f});
}

void World::Update(float inDeltaTime)
{
	for (std::shared_ptr<Entity>& entity : mEntities)
	{
		entity->Update(inDeltaTime);
	}

	UpdatePhysics(inDeltaTime);
}

void World::Render(float inDeltaTime)
{
	gAnimationManager.Update(this, inDeltaTime);

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

EntityPtr World::AddEntity(const EntityPtr& inEntity, const String& inName)
{
	inEntity->AddComponent<NameComponent>(inName);
	mEntities.push_back(inEntity);
	return mEntities.back();
}

b2Body* World::CreatePhysicsBody(const b2BodyDef& inBodyDef, const b2FixtureDef& inFixtureDef)
{
	b2Body* body = mPhysicsWorld->CreateBody(&inBodyDef);
	body->CreateFixture(&inFixtureDef);
	return body;
}

b2Body* World::CreatePhysicsBody(const b2BodyDef& inBodyDef)
{
	return mPhysicsWorld->CreateBody(&inBodyDef);
}

void World::UpdatePhysics(float inDeltaTime)
{
	{
		auto view = mRegistry.view<TransformComponent, PhysicsComponent, PhysicsPositionUpdatedTag>();
		for (auto entity : view)
		{
			fm::Transform2D& transform = view.get<TransformComponent>(entity).mTransform;
			PhysicsComponent& physics = view.get<PhysicsComponent>(entity);

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

			transform.position = {physics.mBody->GetPosition().x, physics.mBody->GetPosition().y};
			transform.rotation = physics.mBody->GetAngle();
		}
	}
}

void World::PhysicsTimeStep()
{
	//Fixed time step for physics world
	mPhysicsWorld->Step(mPhysicsTimeStep, mVelocityIterations, mPositionIterations);
}
