#pragma once
// Core includes
#include <Core/Manager/Manager.h>

namespace sf
{
	class RenderWindow;
}

class Renderer;

struct MsgWindowClosed : public MsgBase
{
	RTTI_MSG()
};

class WindowEventManager : public Manager
{
	RTTI_MANAGER(WindowEventManager, Manager)

public:
	WindowEventManager();
	
	virtual void Update(Renderer& inRenderer, float inDeltaTime);

private:
};