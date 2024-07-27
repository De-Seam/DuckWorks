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
	WindowEventManager();
	
	virtual void Update(Renderer& inRenderer, float inDeltaTime);

private:
};