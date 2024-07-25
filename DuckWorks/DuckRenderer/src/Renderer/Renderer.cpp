#include "Renderer/Precomp.h"
#include <Renderer/Renderer.h>

// Renderer includes
#include <Renderer/WindowEvents/WindowEventManager.h>

// External includes
#include <External/SFML/Graphics.hpp>

Renderer::Renderer()
{
	mWindowEventManager = WindowEventManager::sNewInstance();

	sf::VideoMode video_mode;
	video_mode.width = 1280;
	video_mode.height = 720;
	mRenderWindow = std::make_unique<sf::RenderWindow>(video_mode, "DuckRenderer");
	//mRenderWindow->setVerticalSyncEnabled(true);
	mRenderWindow->setFramerateLimit(60);
	uint32 pixels[8 * 8] =
	{
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0xFFFF00FF, 0xFFFF00FF, 0xFFFF00FF, 0xFFFF00FF, 0x00000000, 0x00000000,
		0x00000000, 0xFFFF00FF, 0xFF0000FF, 0xFF0000FF, 0xFF0000FF, 0xFF0000FF, 0xFFFF00FF, 0x00000000,
		0x00000000, 0xFFFF00FF, 0xFF0000FF, 0xFF00FF00, 0xFF00FF00, 0xFF0000FF, 0xFFFF00FF, 0x00000000,
		0x00000000, 0xFFFF00FF, 0xFF0000FF, 0xFF00FF00, 0xFF00FF00, 0xFF0000FF, 0xFFFF00FF, 0x00000000,
		0x00000000, 0xFFFF00FF, 0xFF0000FF, 0xFF0000FF, 0xFF0000FF, 0xFF0000FF, 0xFFFF00FF, 0x00000000,
		0x00000000, 0x00000000, 0xFFFF00FF, 0xFFFF00FF, 0xFFFF00FF, 0xFFFF00FF, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
	};
	mRenderWindow->setIcon(8, 8, reinterpret_cast<uint8*>(pixels));

	mManagerSettings.mWantsUpdate = true;
}

Renderer::~Renderer()
{
	mRenderWindow->close();
}

void Renderer::Update(float inDeltaTime)
{
	

	sf::View view;
	view.setSize(1920, 1080);
	view.zoom(1.0f);
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
