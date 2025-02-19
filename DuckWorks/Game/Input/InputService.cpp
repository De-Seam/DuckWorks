#include <Game/Input/InputService.h>

#include <DuckCore/Manager/Managers.h>

#include <Engine/Engine.h>

using namespace DC;

InputService::InputService(World& inWorld) :
	Base(inWorld)
{}

InputService::~InputService() = default;

bool InputService::IsMouseButtonDown(EMouseButton inButton) const 
{
	return Managers::sGet<SDLEventManager>().IsMouseButtonDown(inButton);
}

bool InputService::IsKeyDown(SDL_KeyCode inKey) const 
{
	return Managers::sGet<SDLEventManager>().IsKeyDown(inKey);
}

const IVec2& InputService::GetMousePosition() const 
{
	return Managers::sGet<SDLEventManager>().GetMousePosition();
}
