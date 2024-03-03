#include "Precomp.h"
#include "Game/App/FactoryRegistry.h"

// Core includes
#include "Core/Utilities/RTTI.h"

// Engine includes
#include "Engine/Factory/Factory.h"
#include "Engine/Debug/Windows/DebugUIWindowEntitySpawner.h"
#include "Engine/Debug/Windows/DebugUIWindowPerformanceMonitor.h"

// Game includes
#include "Game/Entity/Player/Player.h"

void gRegisterFactoryClasses()
{
	REGISTER_ENTITY(Player);


	REGISTER_DEBUG_UI_WINDOW(DebugUIWindowPerformanceMonitor);
	REGISTER_DEBUG_UI_WINDOW(DebugUIWindowEntitySpawner);
}
