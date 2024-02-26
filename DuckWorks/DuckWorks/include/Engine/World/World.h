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
	void PhysicsTimeStep();
	void Render(float inDeltaTime);

	template<typename taType>
	std::shared_ptr<taType> CreateEntity(const String& inName);
	EntityPtr AddEntity(const EntityPtr& inEntity, const String& inName);

	b2Body* CreatePhysicsBody(const b2BodyDef&, const b2FixtureDef& inFixtureDef);
	b2Body* CreatePhysicsBody(const b2BodyDef& inBodyDef);

	entt::registry& GetRegistry() { return mRegistry; }

private:
	entt::registry mRegistry = {};
	std::unique_ptr<b2World> mPhysicsWorld = nullptr;
	std::vector<EntityPtr> mEntities = {};

	int32 mVelocityIterations = 6;
	int32 mPositionIterations = 2;
	int32 mPhysicsUpdateFrequency = 60; //60 hz
	float mPhysicsTimeStep = 1.0f / Cast<float>(mPhysicsUpdateFrequency);
	float mPhysicsTimeAccumulator = 0.0f;
};

template<typename taType>
std::shared_ptr<taType> World::CreateEntity(const String& inName)
{
	static_assert(std::is_base_of_v<Entity, taType>);

	std::shared_ptr<taType> entity = std::make_shared<taType>(this);
	AddEntity(std::static_pointer_cast<Entity>(entity), inName);
	return entity;
}
