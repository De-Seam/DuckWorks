#pragma once
// DuckCore includes
#include <DuckCore/Containers/Array.h>
#include <DuckCore/Containers/Handle.h>
#include <DuckCore/Containers/HashMap.h>
#include <DuckCore/Containers/UniquePtr.h>
#include <DuckCore/RTTI/RTTIRefClass.h>

#include <Engine/Renderer/Renderer.h>

#define REGISTER_APP(inApp) App::sRegisterAppConstructor(#inApp, []() { return gMove(DC::gMakeUnique<inApp>()); })

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
	taType& TryCreateManager();
	template<typename taType>
	taType& GetManager();
	template<typename taType>
	taType* FindManager();

	[[nodiscard]]
	DC::Ref<EngineUpdateHandle> RegisterUpdateCallback(std::function<void(float)> inCallback);

private:
	void UnregisterUpdateCallback(const EngineUpdateHandle& inHandle);

	bool mIsInitialized = false;
	bool mShouldShutdown = false;

	DC::HashMap<const DC::RTTITypeID, DC::UniquePtr<Manager>> mManagers;
	Renderer* mRenderer = nullptr;

	struct UpdateCallback
	{
		int mID = -1;
		std::function<void(float)> mCallback;
	};
	DC::Array<UpdateCallback> mUpdateCallbacks;

	friend class EngineUpdateHandle;
};

template<typename taType>
taType& Engine::TryCreateManager()
{
	const DC::RTTI& rtti = taType::sGetRTTI();
	
	if (DC::UniquePtr<Manager>* manager = mManagers.Find(rtti.GetTypeID()))
		return *reinterpret_cast<taType*>(manager->Get());

	DC::UniquePtr<taType> manager = DC::gMakeUnique<taType>();
	taType* manager_ptr = manager;
	mManagers[rtti.GetTypeID()] = DC::gMove(manager);

	// Only init if the engine is already initialized. Otherwise, the manager will be initialized in Engine::Init.
	if (mIsInitialized)
		manager_ptr->Init();

	return *manager_ptr;
}

template<typename taType>
taType& Engine::GetManager()
{
	// Default behavior is TryCreate, and return. Specific managers can be overriden, see Renderer etc, for faster getters when we're sure they exist.
	return TryCreateManager<taType>();
}

template<>
inline Renderer& Engine::GetManager<Renderer>()
{
	const DC::RTTI& rtti = Renderer::sGetRTTI();
	
	return *reinterpret_cast<Renderer*>(mManagers[rtti.GetTypeID()].Get());
}

template<typename taType>
taType* Engine::FindManager()
{
	const DC::RTTI& rtti = taType::sGetRTTI();
	
	if (DC::UniquePtr<Manager>* manager = mManagers.Find(rtti.GetTypeID()))
		return reinterpret_cast<taType*>(manager->Get());

	return nullptr;
}
