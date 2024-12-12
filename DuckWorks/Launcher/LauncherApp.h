#pragma once
#include <App/App.h>

class LauncherApp : public App
{
public:
	LauncherApp();
	virtual ~LauncherApp() override;

	virtual void Init() override;
	virtual void Shutdown() override;

	virtual void Update(float inDeltaTime) override;
};