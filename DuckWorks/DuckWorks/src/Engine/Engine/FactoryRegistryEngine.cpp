#include "Precomp.h"
#include "Engine/Engine/FactoryRegistryEngine.h"

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
#include "Engine/Entity/Actor.h"
#include "Engine/Entity/Components.h"
#include "Engine/Entity/Components/CollisionComponent.h"
#include "Engine/Entity/Components/ScriptComponent.h"
#include "Engine/Factory/Factory.h"

void gRegisterFactoryClassesEngine()
{
	PROFILE_SCOPE(gRegisterFactoryClassesEngine)

	// Entities
	REGISTER_ENTITY(Actor);

	// Components
	REGISTER_ENTITY_COMPONENT(TextureRenderComponent);
	REGISTER_ENTITY_COMPONENT(CollisionComponent);
	REGISTER_ENTITY_COMPONENT(AnimationComponent);
	REGISTER_ENTITY_COMPONENT(HealthComponent);
	REGISTER_ENTITY_COMPONENT(CameraComponent);
	REGISTER_ENTITY_COMPONENT(ScriptComponent);

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
