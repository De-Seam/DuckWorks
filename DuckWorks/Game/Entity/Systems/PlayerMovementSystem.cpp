#include <Game/Entity/Systems/PlayerMovementSystem.h>

#include <Game/Entity/Components/GameplayComponents.h>

#include <entt/entity/registry.hpp>

void PlayerMovementSystem::Tick(float inDeltaTime)
{
	// entt::registry& registry = GetRegistry();

	//// Render all sprites
	//registry.view<const PlayerMovementComponent, VelocityComponent, TransformComponent>().each([](const PlayerMovementComponent& inPlayerMovementComponent, VelocityComponent& inVelocityComponent, TransformComponent& inTransformComponent)
	//{
    //    
	//});
}
