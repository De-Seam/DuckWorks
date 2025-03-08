#include <Game/Game.h>

#include <Game/World/Entity/Components/Components.h>
#include <Game/World/Entity/Components/GameplayComponents.h>
#include <Game/World/World.h>

using namespace DC;

void Game::sStartup()
{
	RegisterComponentType<TransformComponent>();
}

Game::Game()
{
	mWorld = new World(GUID::sCreate());
}

Game::~Game() = default;

void Game::Update(float aDeltaTime)
{
	mWorld->Update(aDeltaTime);
}

void Game::Render()
{
	mWorld->Render();
}
