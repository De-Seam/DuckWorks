#pragma once
// Engine includes
#include "Engine/User/UserSettings.h"

// Std includes
#include <memory>

class World;

class App
{
public:
	App();
	~App();

	int Run(); ///< Run app. Returns exit code.
	void Stop(); ///< Stop app.

	void LoadUserSettingsFromFile(const String& inFile);
	void SaveUserSettingsToFile(const String& inFile);

	float GetDeltaTime() const { return mDeltaTime; }

	void SetPaused(bool inPaused) { mPaused = inPaused; }
	bool IsPaused() const { return mPaused; }

	void CreateNewWorld(const Json& inJson);
	World* GetWorld() const { return mWorld.get(); }

	template<typename taType>
	void SetUserSettingsStruct() { mUserSettings = std::unique_ptr<taType>(); }

	BaseUserSettings* GetUserSettings() const { return mUserSettings.get(); }

private:
	void MainLoop();
	void Update(float inDeltaTime);

	void ShutdownInternal(); ///< Internal shutdown app.

private:
	UniquePtr<World> mWorld = nullptr;

	float mDeltaTime = FLT_EPSILON;

	bool mRunning = false;
	bool mPaused = false;

	UniquePtr<BaseUserSettings> mUserSettings;
	String mUserSettingsFile = "UserSettings.json";
};

extern App gApp;
