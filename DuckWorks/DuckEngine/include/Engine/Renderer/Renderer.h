#pragma once
// Core includes
#include <Engine/Manager/Manager.h>
#include <Core/RTTI/RTTI.h>
#include <Core/RTTI/RTTIRefObject.h>

// Std includes
#include <memory>

class WindowEventManager;

namespace sf
{
class Drawable;
class RenderWindow;
}

class Renderer : public RTTIClass
{
	RTTI_CLASS(Renderer, RTTIClass)

public:
	Renderer();
	virtual ~Renderer();

	virtual void Update(float inDeltaTime);

	void BeginFrame();
	void EndFrame();
	void Draw(const sf::Drawable& inDrawable);
	void Draw(const Array<sf::Drawable*>& inDrawables);

	sf::RenderWindow& GetRenderWindow() const { return *mRenderWindow; }

	WindowEventManager& GetWindowEventManager() { return *mWindowEventManager; }

private:
	std::unique_ptr<sf::RenderWindow> mRenderWindow = nullptr;

	Ref<WindowEventManager> mWindowEventManager = nullptr;
};
