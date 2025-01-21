#include <Engine/Engine.h>

#include <Game/Input/InputService.h>

using namespace DC;

InputService::InputService(World& inWorld) :
	Base(inWorld)
{}

InputService::~InputService() = default;

bool InputService::IsMouseButtonDown(EMouseButton inButton) const 
{
	return gEngine->GetManager<SDLEventManager>().IsMouseButtonDown(inButton);
}

bool InputService::IsKeyDown(SDL_KeyCode inKey) const 
{
	return gEngine->GetManager<SDLEventManager>().IsKeyDown(inKey);
}

const IVec2& InputService::GetMousePosition() const 
{
	return gEngine->GetManager<SDLEventManager>().GetMousePosition();
}
