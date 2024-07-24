#include <Renderer/Precomp.h>
#include <Renderer/WindowEvents/WindowEventManager.h>

// Renderer includes
#include <Renderer/Renderer.h>

// External includes
#include <External/SFML/Graphics.hpp>

WindowEventManager::WindowEventManager()
{
	mSettings.AddInitDependency<Renderer>();
}

void WindowEventManager::Update(sf::RenderWindow& inRenderWindow, float)
{
	sf::Event event;
	while (inRenderWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			inRenderWindow.close();
	}
}
