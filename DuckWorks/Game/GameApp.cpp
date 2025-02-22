#include <Game/GameApp.h>

#include <DuckCore/Managers/Managers.h>
#include <DuckCore/Math/Transform.h>

#include <Engine/Renderer/Renderer.h>
#include <Engine/World/World.h>

using namespace DC;

GameApp::GameApp()
{
	mWorld = World::sNew();
}

GameApp::~GameApp() = default;

void GameApp::Update(float inDeltaTime)
{
	mWorld->GetActiveScene().Render();

	SDL_Texture* texture = mWorld->GetActiveScene().GetRenderTarget()->GetTexture();
	Transform2D transform;
	transform.mPosition = { 100.0f, 100.0f };
	transform.mHalfSize.mX = 500.0f / 2.0f;
	transform.mHalfSize.mY = 300.0f / 2.0f;
	Managers::sGet<Renderer>().DrawTexture(texture, transform);
}
