#pragma once
#include "Core/CoreBase.h"

// Engine includes
#include "Engine/Engine/BaseUserSettings.h"

// External includes
#define SOL_ALL_SAFETIES_ON 1
#include "External/sol/sol.hpp"

class World;

class Engine
{
public:
	void Init(UniquePtr<BaseUserSettings> inUserSettings);
	void Shutdown();

	void Update(float inDeltaTime);

	sol::state& GetLua() { return mLua; }
	void CreateNewWorld(const Json& inJson = {});
	World* GetWorld() const { return mWorld.get(); }
	float GetDeltaTime() const { return mDeltaTime; }

	BaseUserSettings* GetUserSettings() const { return mUserSettings.get(); }

	void RequestShutdown() { mShouldShutdown = true; }
	bool ShouldShutdown() const { return mShouldShutdown; }

	void SetPaused(bool inPaused) { mIsPaused = inPaused; }
	bool IsPaused() const { return mIsPaused; }

	void RegisterManager(Manager& inManager);
	void InitManagerBefore(Manager& inManager, Manager& inOtherManager);
	void InitManagerAfter(Manager& inManager, Manager& inOtherManager);
	void UpdateManagerBefore(Manager& inManager, Manager& inOtherManager);
	void UpdateManagerAfter(Manager& inManager, Manager& inOtherManager);

private:
	static void sOrganizeArray(Array<Manager*>& ioArray, const HashMap<Manager*, Array<Manager*>>& inMapBefore, const HashMap<Manager*, Array<Manager*>>& inMapAfter);

	void InitManagers();
	void ShutdownManagers();

	sol::state mLua;

	UniquePtr<World> mWorld = nullptr;

	float mDeltaTime = 0.0f;

	UniquePtr<BaseUserSettings> mUserSettings;

	bool mShouldShutdown = false;
	bool mIsPaused = false;

	Array<Manager*> mManagers;
	HashMap<Manager*, Array<Manager*>> mManagersToInitBeforeMap;
	HashMap<Manager*, Array<Manager*>> mManagersToInitAfterMap;
	Array<Manager*> mManagersToUpdate;
	HashMap<Manager*, Array<Manager*>> mManagersToUpdateBeforeMap;
	HashMap<Manager*, Array<Manager*>> mManagersToUpdateAfterMap;
};

extern Engine gEngine;
