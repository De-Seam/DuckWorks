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

	float targetWidth = 1920.f;
	float targetHeight = 1080.f;

	float windowWidth = static_cast<float>(mRenderWindow->getSize().x);
	float windowHeight = static_cast<float>(mRenderWindow->getSize().y);

	// Calculate aspect ratios
	float windowAspect = windowWidth / windowHeight;
	float viewAspect = targetWidth / targetHeight;

	if (windowAspect > viewAspect) {
		// Window is wider than the view, add black bars on the sides
		float scale = windowHeight / targetHeight;
		float newWidth = targetWidth * scale / windowWidth;
		view.setViewport(sf::FloatRect((1.f - newWidth) / 2.f, 0.f, newWidth, 1.f));
	} else {
		// Window is taller than the view, add black bars on the top and bottom
		float scale = windowWidth / targetWidth;
		float newHeight = targetHeight * scale / windowHeight;
		view.setViewport(sf::FloatRect(0.f, (1.f - newHeight) / 2.f, 1.f, newHeight));
	}

	mRenderWindow->setView(view);

	mWindowEventManager->Update(*this, inDeltaTime);
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
