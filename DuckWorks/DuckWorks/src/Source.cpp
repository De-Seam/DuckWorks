#include "Precomp.h"

// Core includes
#include <Core/Utilities/Assert.h>

// Renderer includes
#include <Engine/Renderer/Renderer.h>

// App includes
#include <App/App.h>
#include <App/AppModule.h>

// External includes
#include <External/SFML/Graphics.hpp>

int main(int, char**)
{
	AppModule app_module;
	app_module.RegisterRTTI();

	App app;
	app.Run();

	Renderer renderer;
	sf::CircleShape shape(800.f);
	shape.setOrigin({300.f,300.f});
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
				shape.setPosition(shape.getPosition().x, shape.getPosition().y + 5);
		}

		renderer.Update(0.0f);
		renderer.Draw(shape);
		renderer.EndFrame();
	}

	return 0;
}

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
