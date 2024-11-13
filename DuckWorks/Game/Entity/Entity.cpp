#include <Game/Entity/Entity.h>

// Game includes
#include <Game/World/World.h>

entt::registry& Entity::GetRegistry() { return mWorld->GetRegistry(); }

const entt::registry& Entity::GetRegistry() const { return mWorld->GetRegistry(); }
