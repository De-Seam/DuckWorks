#pragma once
// Engine includes
#include <Engine/Manager/Manager.h>
#include <Engine/Renderer/WindowEvents/EventMessages.h>

namespace sf
{
	class RenderWindow;
}

class Renderer;         

class WindowEventManager : public Manager
{
	RTTI_MANAGER(WindowEventManager, Manager)

public:
	WindowEventManager() = default;
	WindowEventManager(Renderer& inRenderer);
	
	virtual void Update(Renderer& inRenderer, float inDeltaTime);

	const Vec2& GetMouseDelta() const { return mMouseDelta; }
	const Vec2& GetMousePosition() const { return mMousePosition; }

private:
	Vec2 mMouseDelta = { 0.0f };
	Vec2 mMousePosition = { 0.0f };
};