#include "Precomp.h"
#include "Engine/Entity/Actor.h"

// Engine includes
#include "Engine/Renderer/Renderer.h"
#include "Engine/World/World.h"

// External includes
#include <External/box2d/box2d.h>

Json Actor::Serialize() const
{
	Json json = Base::Serialize();

	return json;
}

void Actor::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);
}


Actor::Actor(World* inWorld)
	: Entity(inWorld)
{
	AddComponent<TransformComponent>();
}

void Actor::CreatePhysicsBody(b2BodyDef inBodyDef, b2FixtureDef inFixtureDef)
{
	gAssert(!HasComponent<PhysicsComponent>(), "This actor already has a physics body!");
	AddComponent<PhysicsComponent>(mWorld->CreatePhysicsBody(inBodyDef, inFixtureDef));
}

void Actor::CreateDefaultPhysicsBody()
{
	b2BodyDef body_def;
	body_def.type = b2_dynamicBody;
	body_def.position = b2Vec2(GetPosition().x, GetPosition().y);
	body_def.angle = GetRotation();
	body_def.linearVelocity = b2Vec2(0.0f, 0.0f);
	body_def.linearDamping = 0.3f;

	b2PolygonShape dynamic_box;
	dynamic_box.SetAsBox(GetHalfSize().x, GetHalfSize().y);

	b2FixtureDef fixture_def;
	fixture_def.shape = &dynamic_box;
	fixture_def.density = 1.0f;
	fixture_def.friction = 1.f;
	fixture_def.restitutionThreshold = 100000.f;

	CreatePhysicsBody(body_def, fixture_def);
}

void Actor::SetVelocity(const fm::vec2& inVelocity)
{
	b2Body* body = GetComponent<PhysicsComponent>().mBody;
	body->SetLinearVelocity({inVelocity.x, inVelocity.y});
}

void Actor::AddVelocity(const fm::vec2& inVelocity)
{
	b2Body* body = GetComponent<PhysicsComponent>().mBody;
	body->SetLinearVelocity(body->GetLinearVelocity() + b2Vec2(inVelocity.x, inVelocity.y));
}

fm::vec2 Actor::GetVelocity()
{
	b2Body* body = GetComponent<PhysicsComponent>().mBody;
	const b2Vec2& velocity = body->GetLinearVelocity();
	return {velocity.x, velocity.y};
}
