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
	//mRenderWindow->setVerticalSyncEnabled(true);
	mRenderWindow->setFramerateLimit(60);

	mManagerSettings.mWantsUpdate = true;
}

Renderer::~Renderer()
{
	mRenderWindow->close();
}

void Renderer::Update(float inDeltaTime)
{
	sf::View view;
	view.setSize(192, 108);
	view.zoom(10.0f);
	view.setCenter({0.0f, 0.0f});

	mRenderWindow->setView(view);
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
