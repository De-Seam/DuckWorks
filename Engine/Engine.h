#pragma once
#include <DuckCore/Containers/Array.h>
#include <DuckCore/Containers/Handle.h>
#include <DuckCore/Containers/HashMap.h>
#include <DuckCore/Containers/UniquePtr.h>
#include <DuckCore/RTTI/RTTIRefClass.h>

#include <Engine/Renderer/Renderer.h>

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

	friend class EngineUpdateHandle;
};
