#include <Game/GameApp.h>
#include <Game/World/World.h>

GameApp::GameApp()
{
}

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
