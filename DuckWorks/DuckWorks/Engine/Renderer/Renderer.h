#pragma once
// Core includes
#include <Engine/Manager/Manager.h>
#include <Core/RTTI/RTTI.h>
#include <Core/RTTI/RTTIRefObject.h>

// Std includes
#include <memory>

struct MsgWindowResized;
struct MsgWindowMouseMoved;
struct MsgWindowMouseWheelScrolled;
class WindowEventManager;

namespace sf
{
class Drawable;
class RenderWindow;
}

struct MsgPreBeginFrame : public MsgBase
{
	RTTI_MSG()
};

struct MsgPostBeginFrame : public MsgBase
{
	RTTI_MSG()
};

struct MsgPreEndFrame : public MsgBase
{
	RTTI_MSG()
};

struct MsgPostEndFrame : public MsgBase
{
	RTTI_MSG()
};


class Renderer : public RTTIClass
{
	RTTI_CLASS(Renderer, RTTIClass)

public:
	Renderer();
	virtual ~Renderer() override;

	virtual Json Serialize() const override;
	virtual void Deserialize(const Json& inJson) override;

	virtual void Update(float inDeltaTime);

	void BeginFrame();
	void EndFrame();
	void Draw(const sf::Drawable& inDrawable);
	void Draw(const Array<sf::Drawable*>& inDrawables);

	sf::RenderWindow& GetRenderWindow() const { return *mRenderWindow; }

	WindowEventManager& GetWindowEventManager() { return *mWindowEventManager; }

protected:
	// Message handlers
	void OnMouseWheelScrolled(const MsgWindowMouseWheelScrolled& inMsg);
	void OnMouseMoved(const MsgWindowMouseMoved& inMsg);
	void OnWindowResized(const MsgWindowResized& inMsg);

private:
	std::unique_ptr<sf::RenderWindow> mRenderWindow = nullptr;

	Ref<WindowEventManager> mWindowEventManager = nullptr;
};
