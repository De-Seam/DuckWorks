#pragma once
// Core includes
#include <Core/Containers/Array.h>
#include <Core/Manager/Manager.h>
#include <Core/RTTI/RTTI.h>

// Std includes
#include <memory>


namespace sf
{
class Drawable;
class RenderWindow;
}

class Renderer : public Manager
{
	RTTI_MANAGER(Renderer, Manager)

public:
	Renderer();
	virtual ~Renderer();

	virtual void Update(float inDeltaTime);

	void BeginFrame();
	void EndFrame();
	void Draw(const sf::Drawable& inDrawable);
	void Draw(const Array<sf::Drawable*>& inDrawables);

	sf::RenderWindow& GetRenderWindow() const { return *mRenderWindow; }

private:
	std::unique_ptr<sf::RenderWindow> mRenderWindow = nullptr;
};
