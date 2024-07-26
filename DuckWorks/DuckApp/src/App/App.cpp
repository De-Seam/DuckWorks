#include <App/Precomp.h>
#include <App/App.h>

// Engine includes
#include <Engine/Renderer/Renderer.h>

// App includes
#include <App/AppModule.h>

// External includes
#include <External/SFML/System/Clock.hpp>

App::App()
{
	gEngine = gMakeUnique<Engine>();
	gEngine->Init();
}

App::~App()
{
	gEngine->Shutdown();
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
	gEngine->Update(inDeltaTime);
}