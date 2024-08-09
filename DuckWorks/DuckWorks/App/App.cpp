#include <Precomp.h>
#include <App/App.h>

// Engine includes
#include <Engine/Renderer/Renderer.h>

// App includes
#include <App/AppModule.h>

// External includes
#include <fstream>

// Std includes
#include <External/SFML/System/Clock.hpp>

App::App()
{
	gAssert(&mEngine == gEngine);
}

App::~App()
{
	gAssert(&mEngine == gEngine);
}

Json App::Serialize() const
{
	Json json = RTTIClass::Serialize();
	JSON_SAVE(json, mEngine);
	return json;
}

void App::Deserialize(const Json& inJson)
{
	RTTIClass::Deserialize(inJson);
	mEngine.Deserialize(inJson["mEngine"]);
}

void App::Run()
{
	{
		std::ifstream file("App.json");
		if (file.is_open())
		{
			Json json = Json::parse(file);
			Deserialize(json);
		}
	}

	gEngine->Init();

	MainLoop();

	gEngine->Shutdown();

	{
		std::ofstream file("App.json");
		if (file.is_open())
		{
			Json json = Serialize();
			file << json.dump(4);
		}
	}
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