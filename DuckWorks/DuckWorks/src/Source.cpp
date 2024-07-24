#include "Precomp.h"

// Core includes
#include <Core/Utilities/Assert.h>

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
	sf::RenderWindow window(sf::VideoMode(350, 350), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
				shape.setPosition(shape.getPosition().x + 1, shape.getPosition().y);
		}

		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}
