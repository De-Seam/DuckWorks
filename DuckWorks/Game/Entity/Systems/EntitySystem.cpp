#include <Game/Entity/Systems/EntitySystem.h>

#include <Game/World/World.h>

entt::registry& EntitySystem::GetRegistry()
{
	return GetWorld().GetRegistry();
}
