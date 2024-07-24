#pragma once
// Core includes
#include <Core/Manager/Manager.h>

namespace sf
{
	class RenderWindow;
}

class WindowEventManager : public Manager
{
	RTTI_MANAGER(WindowEventManager, Manager)

public:
	WindowEventManager();

	virtual void Update(sf::RenderWindow& inRenderWindow, float inDeltaTime);
private:
};