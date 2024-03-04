#pragma once
// Engine includes
#include <Engine/Entity/Actor.h>
#include <Engine/Events/SDLEventManager.h>

class Player : public Actor
{
	RTTI_CLASS(Player, Actor)

public:
	Player(World* inWorld)
		: Actor(inWorld) {}

	virtual void BeginPlay() override;
	virtual void Update(float inDeltaTime) override;

	bool IsAttacking() const { return mAttacking; }

private:
	std::vector<std::shared_ptr<SDLEventFunction>> mSDLEventFunctions;
	float mVelocityIncrement = 600.f; ///< Velocity increment per second
	float mMaxVelocity = 200.f; ///< Maximum velocity
	bool mAttacking = false; ///< Is the player attacking?

private:
	void SetupAnimations();
	void OnMouseDown(const SDL_Event& inEvent);
	void OnMouseUp(const SDL_Event& inEvent);
};
