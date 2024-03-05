#pragma once
// Engine includes
#include <Engine/Entity/Actor.h>
#include <Engine/Events/SDLEventManager.h>

#include "Engine/Events/EventManager.h"

struct EventManager::EventFunction;

class Player : public Actor
{
	RTTI_CLASS(Player, Actor)

public:
	Player(World* inWorld);

	virtual void BeginPlay() override;
	virtual void Update(float inDeltaTime) override;

	bool IsAttacking() const { return mAttacking; }

private:
	Array<SharedPtr<SDLEventFunction>> mSDLEventFunctions;
	Array<SharedPtr<EventManager::EventFunction>> mEventFunctions;
	float mVelocityIncrement = 600.f; ///< Velocity increment per second
	float mMaxVelocity = 200.f; ///< Maximum velocity
	bool mAttacking = false; ///< Is the player attacking?

private:
	void SetupAnimations();
	void OnMouseDown(const EventManager::EventData& inData);
	void OnMouseUp(const EventManager::EventData& inData);
};
