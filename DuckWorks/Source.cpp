// DuckCore includes
#include <DuckCore/Containers/UniquePtr.h>

// Engine includes
#include <Engine/Engine.h>

// App includes
#include <App/App.h>

// Apps includes
#include <CryptChat/CryptChatApp.h>
#include <Editor/EditorApp.h>
#include <Launcher/LauncherApp.h>
#include <Sandbox/SandboxApp.h>

// Std includes
#include <chrono>
#include <DuckCore/Core/Core.h>
#include <Editor/Menus/EntitySpawnerMenu.h>
#include <Editor/Menus/OutlinerMenu.h>
#include <Editor/Menus/ViewportMenu.h>
#include <Engine/Events/SDLEventManager.h>
#include <Engine/Files/FileManager.h>
#include <Engine/Objects/ObjectManager.h>
#include <Engine/Renderer/TextureResource.h>
#include <Engine/Resources/ResourceManager.h>
#include <Game/Entity/Character.h>
#include <Game/Entity/EntityService.h>
#include <Game/Entity/Pawn.h>
#include <Game/Entity/Components/Component.h>
#include <Game/Entity/Components/GameplayComponents.h>
#include <Game/Entity/Components/RenderComponent.h>
#include <Game/World/Service.h>
#include <Game/World/World.h>

void gMainLoop()
{
	// Initial time point
	std::chrono::time_point last_time = std::chrono::high_resolution_clock::now();

	while (!gEngine->ShouldShutdown())
	{
		// Calculate the current time and delta time
		std::chrono::time_point current_time = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> delta_time = current_time - last_time;

		// Max 100 ms, or we start slowing down the application (for debugging etc)
		if (delta_time.count() > 0.1f)
		{
			DC::gLog(DC::LogLevel::Warning, DC::String::sFormatted("Slowing app, delta time took %f seconds", delta_time.count()));
			delta_time = std::chrono::duration<float>(0.1f);
		}

		last_time = current_time;

		gEngine->BeginFrame();

		App::sGetActiveApp()->Update(delta_time.count());

		gEngine->Update(delta_time.count());

		gEngine->EndFrame();
	}
}

void gRegisterRTTI();

/**
Order of initialization:
GameModule constructor: Register classes
Engine constructor: Register managers
App constructor: Register managers / systems
Engine::Init: Initialize managers
**/
int main(int argc, char* argv[])
{
	(void)argc;
	(void)argv;

	gRegisterRTTI();

	Engine engine;

	gEngine->Init();

    REGISTER_APP(LauncherApp);
    REGISTER_APP(GameApp);
    REGISTER_APP(EditorApp);
    REGISTER_APP(SandboxApp);
    REGISTER_APP(CryptChatApp);

	App::sSetActiveApp(DC::gMakeUnique<LauncherApp>());

	gMainLoop();

	App::sClearActiveApp();

	gEngine->Shutdown();

	return 0;
}

void gRegisterRTTI()
{
	DC::gRegisterCoreRTTI();

	REGISTER_RTTI(Object);
	REGISTER_RTTI(EditorMenu);
	REGISTER_RTTI(EntitySpawnerMenu);
	REGISTER_RTTI(OutlinerMenu);
	REGISTER_RTTI(ViewportMenu);
	REGISTER_RTTI(Manager);
	REGISTER_RTTI(SDLEventManager);
	REGISTER_RTTI(FileManager);
	REGISTER_RTTI(ObjectManager);
	REGISTER_RTTI(Renderer);
	REGISTER_RTTI(TextureResource);
	REGISTER_RTTI(Resource);
	REGISTER_RTTI(ResourceManager);
	REGISTER_RTTI(Entity);
	REGISTER_RTTI(World);
	REGISTER_RTTI(Pawn);
	REGISTER_RTTI(Service);
	REGISTER_RTTI(EntityService);
	REGISTER_RTTI(Character);
	REGISTER_RTTI(ComponentBase);
	REGISTER_RTTI(SpriteRenderComponent);
	REGISTER_RTTI(MovementComponent);
	REGISTER_RTTI(BlockingComponent);
	REGISTER_RTTI(VelocityComponent);
	REGISTER_RTTI(TransformComponent);
}

