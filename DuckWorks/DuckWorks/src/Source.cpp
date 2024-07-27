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

	Array<std::thread> threads;
	for (int32 i = 0; i < 2; ++i)
	{
		threads.emplace_back(std::thread([&]()
		{
			App app;
			app.Run();
		}));
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
	}
	
	for (auto& thread : threads)
	{
		thread.join();
	}

	//Renderer renderer;
	//sf::CircleShape shape(800.f);
	//sf::Texture texture;
	//texture.loadFromFile("Assets/DefaultTexture.png");
	//shape.setTexture(&texture);
	//shape.setOrigin({300.f,300.f});
	//
	//while (renderer.GetRenderWindow().isOpen())
	//{
	//	renderer.BeginFrame();
	//	sf::Event event;
	//	while (renderer.GetRenderWindow().pollEvent(event))
	//	{
	//		if (event.type == sf::Event::Closed)
	//			renderer.GetRenderWindow().close();
	//		if (event.type == sf::Event::KeyPressed)
	//			shape.setPosition(shape.getPosition().x, shape.getPosition().y + 5);
	//	}
	//
	//	renderer.Update(0.0f);
	//	renderer.Draw(shape);
	//	renderer.EndFrame();
	//}

	return 0;
}

#ifdef _WIN32

namespace Win
{
#include <Windows.h>
}
int APIENTRY WinMain(
	Win::HINSTANCE,
	Win::HINSTANCE,
	Win::LPSTR, int)
{
	return main(__argc, __argv);
}
#endif
