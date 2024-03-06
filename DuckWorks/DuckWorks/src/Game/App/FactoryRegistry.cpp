#include "Precomp.h"
#include "Game/App/FactoryRegistry.h"

// Core includes
#include "Core/Utilities/RTTI.h"

// Engine includes
#include "Engine/Factory/Factory.h"
#include "Engine/Debug/Windows/DebugUIWindowEntitySpawner.h"
#include "Engine/Debug/Windows/DebugUIWindowPerformanceMonitor.h"
#include "Engine/Debug/Windows/DebugUIWindowEntityDetails.h"
#include "Engine/Debug/Windows/DebugUIWindowEntityList.h"
#include "Engine/Debug/Windows/DebugUIWindowFileExplorer.h"
#include "Engine/Debug/Windows/DebugUIWindowTextureViewer.h"
#include "Engine/Debug/Windows/DebugUIWindowImGuiExample.h"
#include "Engine/Debug/Windows/DebugUIWindowOutputLog.h"

// Game includes
#include "Game/Entity/Player/Player.h"


void gRegisterFactoryClasses()
{
	// Entities
	REGISTER_ENTITY(Actor);
	REGISTER_ENTITY(Player);

	// Components
	REGISTER_COMPONENT(NameComponent);
	REGISTER_COMPONENT(TextureRenderComponent);
	REGISTER_COMPONENT(AnimationComponent);
	REGISTER_COMPONENT(PhysicsComponent);
	REGISTER_COMPONENT(TransformComponent);
	REGISTER_COMPONENT(HealthComponent);
	REGISTER_COMPONENT(PhysicsPositionUpdatedTag);

	// Debug UI Windows
	REGISTER_DEBUG_UI_WINDOW(DebugUIWindowOutputLog);
	REGISTER_DEBUG_UI_WINDOW(DebugUIWindowPerformanceMonitor);
	REGISTER_DEBUG_UI_WINDOW(DebugUIWindowEntitySpawner);
	REGISTER_DEBUG_UI_WINDOW(DebugUIWindowEntityList);
	REGISTER_DEBUG_UI_WINDOW(DebugUIWindowEntityDetails);
	REGISTER_DEBUG_UI_WINDOW(DebugUIWindowFileExplorer);
	REGISTER_DEBUG_UI_WINDOW(DebugUIWindowTextureViewer);
	REGISTER_DEBUG_UI_WINDOW(DebugUIWindowImGuiExample);
}
