#include "Precomp.h"
#include "Game/App/FactoryRegistry.h"

// Core includes
#include "Core/Utilities/RTTI.h"

// Engine includes
#include "Engine/Entity/Components/EntityComponentManager.h"
#include "Engine/Entity/Components.h"
#include "Engine/Factory/Factory.h"
#include "Engine/Debug/Windows/DebugUIWindowEntitySpawner.h"
#include "Engine/Debug/Windows/DebugUIWindowPerformanceMonitor.h"
#include "Engine/Debug/Windows/DebugUIWindowEntityDetails.h"
#include "Engine/Debug/Windows/DebugUIWindowEntityList.h"
#include "Engine/Debug/Windows/DebugUIWindowFileExplorer.h"
#include "Engine/Debug/Windows/DebugUIWindowTextureViewer.h"
#include "Engine/Debug/Windows/DebugUIWindowImGuiExample.h"
#include "Engine/Debug/Windows/DebugUIWindowOutputLog.h"
#include "Engine/Debug/Windows/DebugUIWindowUserSettings.h"
#include "Engine/Debug/Windows/DebugUIWindowWorldSettings.h"
#include "Engine/Debug/Windows/DebugUIWindowEditorToolbar.h"

// Game includes
#include "Game/Entity/SolidObstacle.h"
#include "Game/Entity/Player/Player.h"
#include "Game/Entity/MovingPlatform.h"
#include "Game/Entity/Projectiles/Projectile.h"


void gRegisterFactoryClasses()
{
	PROFILE_SCOPE(gRegisterFactoryClasses)

	// Entities
	REGISTER_ENTITY(Actor);
	REGISTER_ENTITY(Player);
	REGISTER_ENTITY(SolidObstacle);
	REGISTER_ENTITY(MovingPlatform);
	REGISTER_ENTITY(Projectile);

	// Components
	REGISTER_COMPONENT(TextureRenderComponent);
	REGISTER_COMPONENT(CollisionComponent);
	REGISTER_COMPONENT(AnimationComponent);
	REGISTER_COMPONENT(HealthComponent);
	REGISTER_COMPONENT(CameraComponent);

	// Debug UI Windows
	REGISTER_DEBUG_UI_WINDOW(DebugUIWindowOutputLog);
	REGISTER_DEBUG_UI_WINDOW(DebugUIWindowPerformanceMonitor);
	REGISTER_DEBUG_UI_WINDOW(DebugUIWindowEntitySpawner);
	REGISTER_DEBUG_UI_WINDOW(DebugUIWindowEntityList);
	REGISTER_DEBUG_UI_WINDOW(DebugUIWindowEntityDetails);
	REGISTER_DEBUG_UI_WINDOW(DebugUIWindowFileExplorer);
	REGISTER_DEBUG_UI_WINDOW(DebugUIWindowTextureViewer);
	REGISTER_DEBUG_UI_WINDOW(DebugUIWindowUserSettings);
	REGISTER_DEBUG_UI_WINDOW(DebugUIWindowWorldSettings);
	REGISTER_DEBUG_UI_WINDOW(DebugUIWindowImGuiExample);
	REGISTER_DEBUG_UI_WINDOW(DebugUIWindowEditorToolbar);
}
