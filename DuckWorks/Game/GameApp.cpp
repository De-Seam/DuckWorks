#include <Game/GameApp.h>

#include <Engine/World/World.h>

GameApp::GameApp()
{
	mWorld = DC::MakeUnique<World>();
}

GameApp::~GameApp() = default;

void GameApp::Update(float inDeltaTime)
{
	mWorld->Update(inDeltaTime);
}
