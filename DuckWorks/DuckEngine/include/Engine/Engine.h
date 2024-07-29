#pragma once
// Core includes
#include <Core/Containers/Array.h>
#include <Core/Containers/HashMap.h>
#include <Engine/Manager/Manager.h>
#include <Core/RTTI/RTTI.h>
#include <Core/Threads/Thread.h>
#include <Core/Containers/SmartPointers.h>

// Engine includes
#include <Engine/Renderer/Renderer.h>

struct MsgWindowClosed;
class Renderer;

class Engine : public RTTIClass
{
	RTTI_CLASS(Engine, RTTIClass)

public:
	Engine();
	virtual ~Engine() override;

	virtual Json Serialize() const override;
	virtual void Deserialize(const Json& inJson) override;

	void Init();
	void Shutdown();
	void Update(float inDeltaTime);

	void RequestShutdown() { mShutdownRequested = true; }
	bool IsShutdownRequested() const { return mShutdownRequested; }

	template<typename taType>
	void CreateManager();
	void RegisterManager(Manager* inManager); ///< Register a manager without owning it
	template<typename taType>
	taType& GetManager();
	template<typename taType>
	taType* FindManager();

	Renderer& GetRenderer() { return mRenderer; }

protected:
	void OnWindowClosed(const MsgWindowClosed& inMsg);

private:
	void RegisterManagers();

	Array<Ref<Manager>> mManagers;
	Array<Manager*> mManagersToUpdate;

	Renderer mRenderer;

	bool mShutdownRequested = false;

	Json mJson;
};

// Only the main thread of the application has access to the engine
// A new application with its own main thread might create a new engine
// The engine is not accessible from worker threads
THREADLOCAL extern Engine* gEngine;

#include "Engine.inl"
