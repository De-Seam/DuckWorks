#pragma once
#include <DuckCore/Containers/Array.h>
#include <DuckCore/Containers/Handle.h>
#include <DuckCore/Containers/HashMap.h>
#include <DuckCore/Containers/UniquePtr.h>
#include <DuckCore/RTTI/RTTIRefClass.h>

#include <Engine/Renderer/Renderer.h>
#include <Engine/World/World.h>

#define REGISTER_APP(inApp) App::sRegisterAppConstructor(#inApp, []() { return DC::Move(DC::MakeUnique<inApp>()); })

class App;
class Engine;

extern Engine* gEngine;

class Engine
{
public:
	Engine();
	~Engine();

	void Update(float aDeltaTime);

	void RequestShutdown() { mShouldShutdown = true; }
	bool ShouldShutdown() const { return mShouldShutdown; }

private:
	bool mShouldShutdown = false;
	DC::Ref<World> mWorld;

	friend class EngineUpdateHandle;
};
