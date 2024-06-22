#include "Precomp.h"
#include "Game/App/FactoryRegistry.h"

// Core includes
#include "Core/RTTI/RTTI.h"

// Engine includes
#include "Engine/Debug/Windows/DebugUIWindowEditorToolbar.h"
#include "Engine/Debug/Windows/DebugUIWindowEntityDetails.h"
#include "Engine/Debug/Windows/DebugUIWindowEntityList.h"
#include "Engine/Debug/Windows/DebugUIWindowEntitySpawner.h"
#include "Engine/Debug/Windows/DebugUIWindowFileExplorer.h"
#include "Engine/Debug/Windows/DebugUIWindowImGuiExample.h"
#include "Engine/Debug/Windows/DebugUIWindowOutputLog.h"
#include "Engine/Debug/Windows/DebugUIWindowPerformanceMonitor.h"
#include "Engine/Debug/Windows/DebugUIWindowTextureViewer.h"
#include "Engine/Debug/Windows/DebugUIWindowUserSettings.h"
#include "Engine/Debug/Windows/DebugUIWindowWorldSettings.h"
#include "Engine/Entity/Components.h"
#include "Engine/Entity/Components/CollisionComponent.h"
#include "Engine/Entity/Components/EntityComponentManager.h"
#include "Engine/Factory/Factory.h"

// Game includes
#include "Game/Entity/MovingPlatform.h"
#include "Game/Entity/SolidObstacle.h"
#include "Game/Entity/Player/Player.h"
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
	REGISTER_RTTI_CLASS(TextureRenderComponent);
	REGISTER_RTTI_CLASS(CollisionComponent);
	REGISTER_RTTI_CLASS(AnimationComponent);
	REGISTER_RTTI_CLASS(HealthComponent);
	REGISTER_RTTI_CLASS(CameraComponent);

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
