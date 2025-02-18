#include "Precomp.h"
#include "Game/App/FactoryRegistryGame.h"

// Engine includes
#include "Engine/Factory/Factory.h"

// Game includes
#include "Game/Entity/MovingPlatform.h"
#include "Game/Entity/SolidObstacle.h"
#include "Game/Entity/ENemies/Zombie.h"
#include "Game/Entity/Player/Player.h"
#include "Game/Entity/Projectiles/Projectile.h"

using namespace DC;

void gRegisterFactoryClassesGame()
{
	PROFILE_SCOPE(gRegisterFactoryClassesGame)
	gLog(ELogType::Info, "Registering Factory Classes Game");

	// Entities
	REGISTER_ENTITY(Player);
	REGISTER_ENTITY(SolidObstacle);
	REGISTER_ENTITY(MovingPlatform);
	REGISTER_ENTITY(Projectile);
	REGISTER_ENTITY(Zombie);
}
