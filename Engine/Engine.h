#pragma once
#include <DuckCore/Containers/Array.h>
#include <DuckCore/Containers/Handle.h>
#include <DuckCore/Containers/HashMap.h>
#include <DuckCore/Containers/UniquePtr.h>
#include <DuckCore/RTTI/RTTIRefClass.h>

#include <Engine/Renderer/Renderer.h>

#define REGISTER_APP(inApp) App::sRegisterAppConstructor(#inApp, []() { return DC::Move(DC::MakeUnique<inApp>()); })

class App;
class Engine;

extern Engine* gEngine;

class Engine
{
public:
	Engine();
	~Engine();

	void BeginFrame();
	void Update(float inDeltaTime);
	void EndFrame();

	void RequestShutdown() { mShouldShutdown = true; }
	bool ShouldShutdown() const { return mShouldShutdown; }

private:
	bool mShouldShutdown = false;

	friend class EngineUpdateHandle;
};
