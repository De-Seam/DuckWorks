#pragma once
// Engine includes
#include "Game/App/UserSettingsGame.h"

// Std includes
#include <memory>

class World;

class App
{
public:
	App();
	~App();

	int Run(); ///< Run app. Returns exit code.

	void LoadUserSettingsFromFile(const String& inFile, UserSettingsGame& inUserSettings);
	void SaveUserSettingsToFile(const String& inFile, UserSettingsGame& inUserSettings);

private:
	void MainLoop();
	void Update(float inDeltaTime);

	void ShutdownInternal(); ///< Internal shutdown app.

	String mUserSettingsFile = "UserSettings.json";
};

extern App gApp;
