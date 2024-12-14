#include <Game/GameApp.h>
#include <Game/World/World.h>

GameApp::GameApp()
{
	mWorld = DC::gMakeUnique<World>();
}

GameApp::~GameApp() = default;

void GameApp::Update(float inDeltaTime)
{
	mWorld->Update(inDeltaTime);
}
