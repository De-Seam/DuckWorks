#pragma once
#include <Engine/Events/SDLEventManager.h>

#include <Game/World/Service.h>

class InputService : public Service
{
	RTTI_CLASS(InputService, Service)
public:
	InputService(World& inWorld);
	virtual ~InputService() override;

	bool IsMouseButtonDown(EMouseButton inButton) const;
	bool IsKeyDown(SDL_KeyCode inKey) const;

	const DC::IVec2& GetMousePosition() const;
};