#include <Game/GameApp.h>

// Game includes
#include <Game/World/World.h>

GameApp::GameApp() = default;

GameApp::~GameApp() = default;

void GameApp::Init()
{
	mWorld = DC::gMakeUnique<World>();
}

void GameApp::Shutdown()
{
	mWorld.Delete();
}

void GameApp::Update(float inDeltaTime)
{
	mWorld->Update(inDeltaTime);
}
