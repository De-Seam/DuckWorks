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
	gAssert(&mEngine == gEngine);
}

App::~App()
{
	gAssert(&mEngine == gEngine);
}

void App::Run()
{
	gEngine->Init();
	MainLoop();
	gEngine->Shutdown();
}

void App::RequestShutdown()
{
	mShutdownRequested = true;
}

void App::MainLoop()
{
	sf::Clock clock;
	while(!mShutdownRequested && !gEngine->IsShutdownRequested())
	{
		PROFILE_FRAME(App::Frame)
		float delta_time = static_cast<float>(clock.restart().asMicroseconds()) / 1000.f;
		Update(delta_time);
	}
}

void App::Update(float inDeltaTime)
{
	PROFILE_SCOPE(App::Update)
	mEngine.Update(inDeltaTime);
}