#pragma once
// DuckCore includes
#include <DuckCore/Containers/Array.h>
#include <DuckCore/Containers/UniquePtr.h>
#include <DuckCore/RTTI/RTTIRefClass.h>

class Manager;
class Engine;

extern Engine* gEngine;

class Engine
{
public:
	Engine();
	~Engine();

	void Init();
	void Shutdown();

	void Update(float inDeltaTime);

	void RequestShutdown() { mShouldShutdown = true; }
	bool ShouldShutdown() const { return mShouldShutdown; }

private:
	bool mShouldShutdown = false;

	DC::Array<DC::UniquePtr<Manager>> mManagers;
};