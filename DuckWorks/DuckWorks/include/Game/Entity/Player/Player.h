#pragma once
// Engine includes
#include "Engine/Entity/Actor.h"
#include "Engine/Events/EventManager.h"
#include "Engine/Events/SDLEventManager.h"

struct EventManager::EventFunction;
struct CollisionFuncParams;

class Player : public Actor
{
	RTTI_CLASS(Player, Actor)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	Player(const ConstructParameters& inConstructParameters);

	virtual void BeginPlay() override;
	virtual void Update(float inDeltaTime) override;

	bool IsAttacking() const { return mAttacking; }
	Vec2 GetVelocity() const { return mVelocity; }

private:
	Array<SharedPtr<SDLEventFunction>> mSDLEventFunctions;
	Array<SharedPtr<EventManager::EventFunction>> mEventFunctions;
	Vec2 mVelocityIncrement = {2000.f, 2000.f}; ///< Velocity increment per second
	Vec2 mVelocity = {0.f};
	float mMaxVelocity = 200.f; ///< Maximum velocity
	Vec2 mVelocityDecrement = {1000.f, 1000.f}; ///< Velocity decrement per second
	bool mAttacking = false; ///< Is the player attacking?

private:
	void SetupAnimations();
	void OnMouseDown(const EventManager::EventData& inData);
	void OnMouseUp(const EventManager::EventData& inData);

	void OnCollision(const CollisionFuncParams& inParams);
};
