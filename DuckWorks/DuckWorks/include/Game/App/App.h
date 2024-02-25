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
	void Stop(); ///< Stop app.

	float GetDeltaTime() const { return mDeltaTime; }

private:
	void MainLoop();
	void Update(float inDeltaTime);

	void ShutdownInternal(); ///< Internal shutdown app.

private:
	std::unique_ptr<World> mWorld;

	float mDeltaTime = FLT_EPSILON;
	bool mRunning = false;
};

extern App gApp;
