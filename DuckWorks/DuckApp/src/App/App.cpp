#include <App/Precomp.h>
#include <App/App.h>

// Renderer includes
#include <Renderer/Renderer.h>

// External includes
#include <External/SFML/System/Clock.hpp>

App::App()
{
	mEngine.Init();
}

App::~App()
{
	mEngine.Shutdown();
}

void App::Run()
{
	MainLoop();
}

void App::RequestShutdown()
{
	mShutdownRequested = true;
}

void App::MainLoop()
{
	sf::Clock clock;
	while(!mShutdownRequested)
	{
		float delta_time = static_cast<float>(clock.restart().asMicroseconds()) / 1000.f;
		Update(delta_time);
	}
}

void App::Update(float inDeltaTime)
{
	mEngine.Update(inDeltaTime);
}