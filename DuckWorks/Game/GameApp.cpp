#include <Game/GameApp.h>

#include <Game/Entity/Character.h>
#include <Game/Entity/EntityService.h>
#include <Game/Entity/Pawn.h>
#include <Game/Entity/Components/GameplayComponents.h>
#include <Game/Entity/Components/RenderComponent.h>
#include <Game/World/World.h>

void GameApp::sRegisterRTTI() 
{
	REGISTER_RTTI(Entity);
	REGISTER_RTTI(World);
	REGISTER_RTTI(Pawn);
	REGISTER_RTTI(Service);
	REGISTER_RTTI(EntityService);
	REGISTER_RTTI(Character);
	REGISTER_RTTI(ComponentBase);
	REGISTER_RTTI(SpriteRenderComponent);
	REGISTER_RTTI(MovementComponent);
	REGISTER_RTTI(BlockingComponent);
	REGISTER_RTTI(VelocityComponent);
	REGISTER_RTTI(TransformComponent);
}

GameApp::GameApp()
{
	mWorld = DC::gMakeUnique<World>();
}

GameApp::~GameApp() = default;

void GameApp::Update(float inDeltaTime)
{
	mWorld->Update(inDeltaTime);
}
