#pragma once
#include <App/App.h>

class LauncherApp : public App
{
public:
	LauncherApp();
	virtual ~LauncherApp() override;

	virtual void Update(float inDeltaTime) override;
};