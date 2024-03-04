#pragma once
#include "Entity.h"
#include "External/box2d/b2_body.h"

// An actor is an entity that has a transform component. It can be moved and rotated, and physics can be added
class Actor : public Entity
{
	RTTI_CLASS(Actor, Entity)

public:
	Actor(World* inWorld);

	void CreatePhysicsBody(b2BodyDef inBodyDef, b2FixtureDef inFixtureDef);
	void CreateDefaultPhysicsBody();

	void SetTransform(const fm::Transform2D& transform) { GetComponent<TransformComponent>().mTransform = transform; }
	void SetPosition(const fm::vec2& inPosition) { GetComponent<TransformComponent>().mTransform.position = inPosition; }
	void SetHalfSize(const fm::vec2& inHalfSize) { GetComponent<TransformComponent>().mTransform.halfSize = inHalfSize; }
	void SetRotation(float inRotation) { GetComponent<TransformComponent>().mTransform.rotation = inRotation; }
	void SetVelocity(const fm::vec2& inVelocity);
	void AddVelocity(const fm::vec2& inVelocity); ///< Adds to the current velocity

	[[nodiscard]] TransformComponent& GetTransformComponent() { return GetComponent<TransformComponent>(); }
	[[nodiscard]] fm::Transform2D& GetTransformRef() { return GetComponent<TransformComponent>().mTransform; }
	[[nodiscard]] const fm::Transform2D& GetTransform() { return GetComponent<TransformComponent>().mTransform; }
	[[nodiscard]] const fm::vec2& GetPosition() { return GetComponent<TransformComponent>().mTransform.position; }
	[[nodiscard]] const fm::vec2& GetHalfSize() { return GetComponent<TransformComponent>().mTransform.halfSize; }
	[[nodiscard]] float GetRotation() { return GetComponent<TransformComponent>().mTransform.rotation; }

	[[nodiscard]] b2Body* GetPhysicsBody() { return GetComponent<PhysicsComponent>().mBody; }
	[[nodiscard]] fm::vec2 GetVelocity();

private:
};
