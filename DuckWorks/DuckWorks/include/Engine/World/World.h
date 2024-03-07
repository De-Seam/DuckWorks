#pragma once
#include "Core/Utilities/Utilities.h"

// Engine includes
#include "Engine/Entity/Entity.h"

// External includes
#include <External/entt/entt.hpp>
#include <External/box2d/b2_world.h>
#include "External/box2d/b2_fixture.h"

class b2World;

class World : public RTTIBaseClass
{
	RTTI_CLASS(World, RTTIBaseClass)

public:
	World();

	Json Serialize();
	void Deserialize(const Json& inJson);

	void Update(float inDeltaTime);
	void Render(float inDeltaTime);

	void UpdateEntities(float inDeltaTime);
	void DestroyEntities();

	template<typename taType>
	SharedPtr<taType> CreateEntity(const String& inName);
	EntityPtr AddEntity(const EntityPtr& inEntity, const String& inName);

	b2Body* CreatePhysicsBody(const b2BodyDef&, const b2FixtureDef& inFixtureDef);
	b2Body* CreatePhysicsBody(const b2BodyDef& inBodyDef);

	Array<EntityPtr>& GetEntities() { return mEntities; }
	const Array<EntityPtr>& GetEntities() const { return mEntities; }
	entt::registry& GetRegistry() { return mRegistry; }
	b2World* GetPhysicsWorld() { return mPhysicsWorld.get(); }

	EntityPtr GetEntityAtLocationSlow(fm::vec2 inWorldLocation);

private:
	entt::registry mRegistry = {};
	UniquePtr<b2World> mPhysicsWorld = nullptr;
	Mutex mPhysicsWorldMutex = {};

	Array<EntityPtr> mEntities = {};
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

	std::shared_ptr<taType> entity = std::make_shared<taType>(this);
	entity->mThisWeakPtr = entity;
	AddEntity(std::static_pointer_cast<Entity>(entity), inName);
	return entity;
}
