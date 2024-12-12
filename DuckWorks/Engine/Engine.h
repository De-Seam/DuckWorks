#pragma once
// DuckCore includes
#include <DuckCore/Containers/Array.h>
#include <DuckCore/Containers/Handle.h>
#include <DuckCore/Containers/HashMap.h>
#include <DuckCore/Containers/UniquePtr.h>
#include <DuckCore/RTTI/RTTIRefClass.h>

#define REGISTER_APP(inApp) gEngine->RegisterApp(#inApp, []() { return gMove(DC::gMakeUnique<inApp>()); })

class App;
class Engine;
class Manager;

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

	void Init();
	void Shutdown();

	void BeginFrame();
	void Update(float inDeltaTime);
	void EndFrame();

	void RequestShutdown() { mShouldShutdown = true; }
	bool ShouldShutdown() const { return mShouldShutdown; }

	template<typename taType>
	void TryCreateManager();
	template<typename taType>
	taType& GetManager();
	template<typename taType>
	taType* FindManager();

	[[nodiscard]]
	DC::Ref<EngineUpdateHandle> RegisterUpdateCallback(std::function<void(float)> inCallback);

	void SetApp(DC::UniquePtr<App> inApp);
	void RegisterApp(const DC::String& inName, std::function<DC::UniquePtr<App>(void)> inConstructFunction);
	const DC::HashMap<DC::String, std::function<DC::UniquePtr<App>(void)>>& GetApps() const { return mApps; }

private:
	void UnregisterUpdateCallback(const EngineUpdateHandle& inHandle);

	bool mShouldShutdown = false;

	DC::UniquePtr<App> mApp;
	DC::HashMap<DC::String, std::function<DC::UniquePtr<App>(void)>> mApps; // Maps App name to constructor function

	DC::HashMap<const DC::RTTITypeID, DC::UniquePtr<Manager>> mManagers;

	struct UpdateCallback
	{
		int mID = -1;
		std::function<void(float)> mCallback;
	};
	DC::Array<UpdateCallback> mUpdateCallbacks;

	friend class EngineUpdateHandle;
};

template<typename taType>
void Engine::TryCreateManager()
{
	const DC::RTTI& rtti = taType::sGetRTTI();

	if (mManagers.Contains(rtti.GetTypeID()))
		return;
	
	mManagers[rtti.GetTypeID()] = DC::gMove(DC::gMakeUnique<taType>());
}

template<typename taType>
taType& Engine::GetManager()
{
	const DC::RTTI& rtti = taType::sGetRTTI();
	
	if (DC::UniquePtr<Manager>* manager = mManagers.Find(rtti.GetTypeID()))
		return *reinterpret_cast<taType*>(manager->Get());

	DC::UniquePtr<taType> manager = DC::gMakeUnique<taType>();
	taType* manager_ptr = manager;
	mManagers[rtti.GetTypeID()] = DC::gMove(manager);
	return *manager_ptr;
}

template<typename taType>
taType* Engine::FindManager()
{
	const DC::RTTI& rtti = taType::sGetRTTI();
	
	if (DC::UniquePtr<Manager>* manager = mManagers.Find(rtti.GetTypeID()))
		return reinterpret_cast<taType*>(manager->Get());

	return nullptr;
}
