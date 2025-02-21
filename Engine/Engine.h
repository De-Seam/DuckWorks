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

class EngineUpdateHandle : public DC::Handle
{
public:
	virtual ~EngineUpdateHandle() override;

private:
	EngineUpdateHandle(int inID) : Handle(inID) {}

	friend class Engine;
};

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

	[[nodiscard]]
	DC::Ref<EngineUpdateHandle> RegisterUpdateCallback(std::function<void(float)> inCallback);

private:
	void UnregisterUpdateCallback(const EngineUpdateHandle& inHandle);

	bool mShouldShutdown = false;

	struct UpdateCallback
	{
		int mID = -1;
		std::function<void(float)> mCallback;
	};
	DC::Array<UpdateCallback> mUpdateCallbacks;

	friend class EngineUpdateHandle;
};
