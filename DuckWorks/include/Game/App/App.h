#pragma once
// Engine includes
#include "Game/App/UserSettingsGame.h"

class App
{
public:
	App();
	~App();

	int Run(); ///< Run app. Returns exit code.

	void LoadUserSettingsFromFile(const DC::String& inFile, UserSettingsGame& inUserSettings);
	void SaveUserSettingsToFile(const DC::String& inFile, UserSettingsGame& inUserSettings);

private:
	void MainLoop();
	void Update(float inDeltaTime);

	void ShutdownInternal(); ///< Internal shutdown app.

	DC::String mUserSettingsFile = "UserSettings.json";
};

extern App gApp;
