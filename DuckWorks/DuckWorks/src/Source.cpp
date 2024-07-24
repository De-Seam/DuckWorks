#include "Precomp.h"

// Core includes
#include <Core/Utilities/Assert.h>

// Renderer includes
#include <Renderer/Renderer.h>

// External includes
#include <External/SFML/Graphics.hpp>

int main(int argc, char** argv);

#ifdef _WIN32

#include <Windows.h>

int APIENTRY WinMain(
	HINSTANCE,
	HINSTANCE,
	LPSTR, int)
{
	return main(__argc, __argv);
}
#endif

int main(int, char**)
{
	Renderer renderer;
	Renderer renderer2;
	Renderer renderer3;
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	while (renderer.GetRenderWindow().isOpen())
	{
		renderer.BeginFrame();
		sf::Event event;
		while (renderer.GetRenderWindow().pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				renderer.GetRenderWindow().close();
			if (event.type == sf::Event::KeyPressed)
				shape.setPosition(shape.getPosition().x + 1, shape.getPosition().y);
		}

		renderer.Draw(shape);
		renderer2.BeginFrame();
		renderer2.Draw(shape);
		renderer3.BeginFrame();
		renderer3.Draw(shape);
		renderer.EndFrame();

		renderer2.EndFrame();

		renderer3.EndFrame();
	}

	return 0;
}
