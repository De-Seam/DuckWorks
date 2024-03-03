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

	void SetPaused(bool inPaused) { mPaused = inPaused; }
	bool IsPaused() const { return mPaused; }

	World* GetWorld() const { return mWorld.get(); }

private:
	void MainLoop();
	void Update(float inDeltaTime);

	void ShutdownInternal(); ///< Internal shutdown app.

private:
	std::unique_ptr<World> mWorld;

	float mDeltaTime = FLT_EPSILON;
	bool mRunning = false;
	bool mPaused = false;
};

extern App gApp;
