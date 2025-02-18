#include "Precomp.h"
// Own include
#include "Game/App/App.h"

// Engine includes
#include "Engine/Debug/DebugUIWindowManager.h"
#include "Engine/Debug/Windows/DebugUIWindowEditorToolbar.h"
#include "Engine/Engine/Engine.h"

// Game includes
#include "Game/App/FactoryRegistryGame.h"

// Std includes
#include <fstream>

using namespace DC;

App gApp;

App::App()
{
	assert(this == &gApp);
}

App::~App()
{
	assert(this == &gApp);
}

int App::Run()
{
	OPTICK_FRAME("Initialize") ///< Main thread so we can profile up until the first frame

	PROFILE_SCOPE(App::Run)

	gLog(ELogType::Info, "Initializing App");

	UniquePtr<BaseUserSettings> user_settings = std::make_unique<UserSettingsGame>();
	LoadUserSettingsFromFile(mUserSettingsFile, *gCast<UserSettingsGame>(user_settings.get()));

	gEngine.Init(std::move(user_settings));

	gRegisterFactoryClassesGame(); {} {}

	std::ifstream in_file("world.json");
	Json world_json = {};
	if (in_file.is_open())
		in_file >> world_json;
	gEngine.CreateNewWorld(world_json);

	MainLoop();

	ShutdownInternal();
	return 0;
}

void App::LoadUserSettingsFromFile(const String& inFile, UserSettingsGame& inUserSettings)
{
	PROFILE_SCOPE(App::LoadUserSettingsFromFile)
	gLog(ELogType::Info, "Loading User Settings From File");

	mUserSettingsFile = inFile;

	std::ifstream file(inFile);
	if (!file.is_open())
		return gLog(ELogType::Warning, "Failed to open user settings file: %s. A default one will be created when the program shuts down", inFile.c_str());

	Json json = Json::parse(file);
	inUserSettings.Deserialize(json);
}

void App::SaveUserSettingsToFile(const String& inFile, UserSettingsGame& inUserSettings)
{
	PROFILE_SCOPE(App::SaveUserSettingsToFile)
	gLog(ELogType::Info, "Saving User Settings To File");

	mUserSettingsFile = inFile;

	std::ofstream file(inFile);
	Json json = inUserSettings.Serialize();
	file << json.dump(4);
}

void App::MainLoop()
{
	auto last_time = std::chrono::steady_clock::now();
	while (!gEngine.ShouldShutdown())
	{
		auto current_time = std::chrono::steady_clock::now();
		float delta_time = std::chrono::duration_cast<std::chrono::duration<float>>(current_time - last_time).count();

		double min_delta_time = 1.0 / SCast<float>(gEngine.GetUserSettings()->mMaxFPS);
		if (delta_time >= min_delta_time)
		{
			OPTICK_FRAME("MainThread")

			Update(delta_time);

			last_time = current_time;
		}
	}
}

void App::Update(float inDeltaTime)
{
	PROFILE_SCOPE(App::Update)

	gEngine.Update(inDeltaTime);
}

void App::ShutdownInternal()
{
	PROFILE_SCOPE(App::ShutdownInternal)
	gLog(ELogType::Info, "App Shutting Down");

	Ref<DebugUIWindowEditorToolbar> toolbar = gDebugUIWindowManager.GetWindow<DebugUIWindowEditorToolbar>();
	// If the state was not stopped, we can skip re-saving the world
	if (toolbar->GetGameState() == ToolbarGameState::Stopped)
		toolbar->Save();
	toolbar->SaveStateToFile();

	gEngine.Deinitialize();

	SaveUserSettingsToFile(mUserSettingsFile, *gCast<UserSettingsGame>(gEngine.GetUserSettings()));
}
