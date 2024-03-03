#pragma once
#include "Core/Utilities/Utilities.h"

// Engine includes
#include "Engine/Entity/Entity.h"

// External includes
#include <External/entt/entt.hpp>
#include <External/box2d/b2_world.h>
#include "External/box2d/b2_fixture.h"

class b2World;

class World
{
public:
	World();

	void Update(float inDeltaTime);
	void Render(float inDeltaTime);

	template<typename taType>
	SharedPtr<taType> CreateEntity(const String& inName);
	EntityPtr AddEntity(const EntityPtr& inEntity, const String& inName);

	b2Body* CreatePhysicsBody(const b2BodyDef&, const b2FixtureDef& inFixtureDef);
	b2Body* CreatePhysicsBody(const b2BodyDef& inBodyDef);

	entt::registry& GetRegistry() { return mRegistry; }
	b2World* GetPhysicsWorld() { return mPhysicsWorld.get(); }

private:
	entt::registry mRegistry = {};
	UniquePtr<b2World> mPhysicsWorld = nullptr;
	Mutex mPhysicsWorldMutex = {};

	std::vector<EntityPtr> mEntities = {};
	Mutex mEntitiesMutex = {};

	int32 mVelocityIterations = 6;
	int32 mPositionIterations = 2;
	int32 mPhysicsUpdateFrequency = 60; //60 hz
	float mPhysicsTimeStep = 1.0f / Cast<float>(mPhysicsUpdateFrequency);
	float mPhysicsTimeAccumulator = 0.0f;

private:
	void UpdatePhysics(float inDeltaTime);
	void PhysicsTimeStep();
};

template<typename taType>
std::shared_ptr<taType> World::CreateEntity(const String& inName)
{
	static_assert(std::is_base_of_v<Entity, taType>);

	std::shared_ptr<taType> entity = std::make_shared<taType>();
	entity->GenerateNewEntityHandle(this);
	entity->mThisWeakPtr = entity;
	AddEntity(std::static_pointer_cast<Entity>(entity), inName);
	return entity;
}
