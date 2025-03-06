#include <Game/Game.h>

#include <Game/World/World.h>

using namespace DC;

Game::Game()
{
	mWorld = new World(GUID::sCreate());
}

Game::~Game() = default;
