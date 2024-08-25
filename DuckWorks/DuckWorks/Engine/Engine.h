#pragma once
// Core includes
#include <Core/Containers/Array.h>
#include <Engine/Manager/Manager.h>
#include <Core/RTTI/RTTI.h>
#include <Core/Containers/SmartPointers.h>

// Engine includes
#include <Engine/Renderer/Renderer.h>
#include <Engine/World/World.h>

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
	World* GetWorld() const { return mWorld.get(); }

protected:
	void OnWindowClosed(const MsgWindowClosed& inMsg);

private:
	void RegisterManagers();

	Array<Ref<Manager>> mManagers;
	Array<Manager*> mManagersToUpdate;

	Renderer mRenderer;

	UniquePtr<World> mWorld = nullptr;

	bool mShutdownRequested = false;

	Json mJson;
};

extern Engine* gEngine;

#include "Engine.inl"
