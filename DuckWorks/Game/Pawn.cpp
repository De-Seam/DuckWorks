#include <Game/Entity/Pawn.h>

#include <Game/Entity/Components/GameplayComponents.h>
#include <Game/Entity/Components/RenderComponent.h>

using namespace DC;

Pawn::Pawn(World& inWorld) : 
	Base(inWorld)
{
	AddComponent<TransformComponent>();
	AddComponent<VelocityComponent>();
	AddComponent<MovementComponent>();
	AddComponent<SpriteRenderComponent>();
}