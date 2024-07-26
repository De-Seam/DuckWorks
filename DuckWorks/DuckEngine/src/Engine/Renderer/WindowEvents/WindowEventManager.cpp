#include <Engine/Precomp.h>
#include <Engine/Renderer/WindowEvents/WindowEventManager.h>

// Core includes
#include <Core/Utilities/Assert.h>

// Engine includes
#include <Engine/Renderer/Renderer.h>

// External includes
#include <External/SFML/Graphics.hpp>

WindowEventManager::WindowEventManager()
{
	mManagerSettings.AddInitDependency<Renderer>();
}

void WindowEventManager::Update(Renderer& inRenderer, float)
{
	sf::RenderWindow& render_window = inRenderer.GetRenderWindow();
	sf::Event event;
	while (render_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			render_window.close();
	}
}
