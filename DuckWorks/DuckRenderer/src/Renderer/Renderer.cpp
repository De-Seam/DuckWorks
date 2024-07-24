#include "Renderer/Precomp.h"
#include <Renderer/Renderer.h>

// External includes
#include <External/SFML/Graphics.hpp>

Renderer::Renderer()
{
	sf::VideoMode video_mode;
	video_mode.width = 1280;
	video_mode.height = 720;
	mRenderWindow = std::make_unique<sf::RenderWindow>(video_mode, "DuckRenderer");
}

Renderer::~Renderer()
{
	mRenderWindow->close();
}

void Renderer::BeginFrame()
{
	mRenderWindow->clear();
}

void Renderer::EndFrame()
{
	mRenderWindow->display();
}

void Renderer::Draw(const sf::Drawable& inDrawable)
{
	mRenderWindow->draw(inDrawable);
}

void Renderer::Draw(const Array<sf::Drawable*>& inDrawables)
{
	for (const sf::Drawable* drawable : inDrawables)
		mRenderWindow->draw(*drawable);
}
