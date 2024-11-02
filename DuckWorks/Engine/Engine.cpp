#include "Engine.h"

Engine* gEngine = nullptr;

Engine::Engine()
{
	gAssert(gEngine == nullptr);
	gEngine = this;
}

Engine::~Engine()
{
	gAssert(gEngine == this);
	gEngine = nullptr;
}

void Engine::Update(float inDeltaTime)
{
	
}
