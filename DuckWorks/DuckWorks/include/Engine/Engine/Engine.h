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

	void Update(float inDeltaTime);

	bool FileExists(const char* inFilePath) const;

	sol::state& GetLua() { return mLua; }
	void CreateNewWorld(const Json& inJson = {});
	World* GetWorld() const { return mWorld.get(); }
	float GetDeltaTime() const { return mDeltaTime; }

	BaseUserSettings* GetUserSettings() const { return mUserSettings.get(); }

	void Shutdown() { mShouldShutdown = true; }
	bool ShouldShutdown() const { return mShouldShutdown; }

	void SetPaused(bool inPaused) { mIsPaused = inPaused; }
	bool IsPaused() const { return mIsPaused; }

	void Deinitialize();

private:
	sol::state mLua;

	UniquePtr<World> mWorld = nullptr;

	float mDeltaTime = 0.0f;

	UniquePtr<BaseUserSettings> mUserSettings;

	bool mShouldShutdown = false;
	bool mIsPaused = false;
};

extern Engine gEngine;
