#pragma once
// DuckCore includes
#include <DuckCore/Containers/Array.h>
#include <DuckCore/Containers/Handle.h>
#include <DuckCore/Containers/HashMap.h>
#include <DuckCore/Containers/UniquePtr.h>
#include <DuckCore/RTTI/RTTIRefClass.h>

class Manager;
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

	void Init();
	void Shutdown();

	void Update(float inDeltaTime);

	void RequestShutdown() { mShouldShutdown = true; }
	bool ShouldShutdown() const { return mShouldShutdown; }

	template<typename taType>
	void CreateManager();
	template<typename taType>
	void RemoveManager();

	template<typename taType>
	taType& GetManager();

	[[nodiscard]]
	DC::Ref<EngineUpdateHandle> RegisterUpdateCallback(std::function<void(float)> inCallback);

private:
	void UnregisterUpdateCallback(const EngineUpdateHandle& inHandle);

	bool mShouldShutdown = false;

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
void Engine::CreateManager()
{
	const DC::RTTI& rtti = taType::sGetRTTI();
	gAssert(!mManagers.Contains(rtti.GetTypeID()));
	mManagers[rtti.GetTypeID()] = DC::UniquePtr<taType>::sMakeUnique();
}

template<typename taType>
void Engine::RemoveManager()
{
	gVerify(mManagers.Remove(taType::sGetRTTI().GetTypeID()));
}

template<typename taType>
taType& Engine::GetManager()
{
	Manager* manager = mManagers[taType::sGetRTTI().GetTypeID()];
	return *reinterpret_cast<taType*>(manager);
}
