#include "Precomp.h"
#include "Game/App/FactoryRegistry.h"

// Core includes
#include "Core/Utilities/RTTI.h"

// Engine includes
#include "Engine/Entity/EntityFactory.h"

// Game includes
#include "Game/Entity/Player/Player.h"

void gRegisterFactoryClasses()
{
	REGISTER_ENTITY(Player);
}
