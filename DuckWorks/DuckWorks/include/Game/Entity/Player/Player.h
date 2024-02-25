#pragma once
// Engine includes
#include <Engine/Entity/Actor.h>
#include <Engine/Events/SDLEventManager.h>

class Player : public Actor
{
	using Base = Actor;

public:
	Player(World* inWorld);

	virtual void Update(float inDeltaTime) override;

private:
	std::shared_ptr<SDLEventFunction> mSDLEventFunction;

private:
	void OnKeyDown(const SDL_Event& inEvent);
};
