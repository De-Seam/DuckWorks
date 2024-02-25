#pragma once

// Std includes
#include <memory>

class World;

class App
{
public:
	App();
	~App();

	int Run(); ///< Run app. Returns exit code.

private:
	void MainLoop();
	void Update(float inDeltaTime);

private:
	std::unique_ptr<World> mWorld;

	float mDeltaTime = FLT_EPSILON;
	bool mRunning = false;
};

extern App gApp;
