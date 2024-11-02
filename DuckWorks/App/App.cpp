#include <App/App.h>

// Core includes
#include <DuckCore/Containers/SharedPtr.h>

// Engine includes
#include <Engine/Engine.h>

App::App()
{
	if (gEngine == nullptr)
		mEngine = new Engine;
	else
		mEngine = gEngine;
}

App::~App()
{
	mEngine = nullptr;
}

void App::Update(float inDeltaTime)
{
	
}

bool App::ShouldShutdown() const
{
	return gEngine->ShouldShutdown();
}
