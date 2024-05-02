#pragma once
#include "Entity.h"

// An actor is an entity that has a transform component. It can be moved and rotated
class Actor : public Entity
{
	RTTI_CLASS(Actor, Entity)

public:
	Actor() = default;
	virtual void Init(const InitParams& inInitParams) override;

	virtual void SetTransform(const fm::Transform2D& transform);
	virtual void SetPosition(const fm::vec2& inPosition);
	virtual void SetHalfSize(const fm::vec2& inHalfSize);
	virtual void SetRotation(float inRotation);

	[[nodiscard]] MutexReadProtectedPtr<TransformComponent> GetTransformComponent();
	[[nodiscard]] fm::Transform2D GetTransform();
	[[nodiscard]] fm::vec2 GetPosition();
	[[nodiscard]] fm::vec2 GetHalfSize();
	[[nodiscard]] float GetRotation();
};
