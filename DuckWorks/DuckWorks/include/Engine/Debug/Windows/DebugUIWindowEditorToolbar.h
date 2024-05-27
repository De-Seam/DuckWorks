#pragma once
#include "DebugUIWindow.h"

enum class ToolbarGameState
{
	Playing,
	Paused,
	Stopped
};

class DebugUIWindowEditorToolbar : public DebugUIWindow
{
	RTTI_CLASS(DebugUIWindowEditorToolbar, DebugUIWindow, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	using Base::Base;

	DebugUIWindowEditorToolbar(const ConstructParameters& inConstructParameters = {});

	virtual void UpdateMultiThreaded(float inDeltaTime) override;
	virtual void Update(float inDeltaTime) override;

	ToolbarGameState GetGameState() const { return mGameState; }

	void StopPlay();
	void Save();
	void SaveStateToFile();

private:
	SharedPtr<TextureResource> mSaveButtonTexture = nullptr;
	SharedPtr<TextureResource> mPlayButtonTexture = nullptr;
	SharedPtr<TextureResource> mPauseButtonTexture = nullptr;
	SharedPtr<TextureResource> mStopButtonTexture = nullptr;

	Json mWorldJson;

	// Default to stopped. Switching to and from Stopped triggers saving or loading the world.
	ToolbarGameState mGameState = ToolbarGameState::Stopped;
};
