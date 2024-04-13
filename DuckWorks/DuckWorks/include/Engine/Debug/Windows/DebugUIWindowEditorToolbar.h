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
	RTTI_CLASS(DebugUIWindowEditorToolbar, DebugUIWindow)

public:
	DebugUIWindowEditorToolbar();

	virtual void UpdateMultiThreaded(float inDeltaTime) override;
	virtual void Update(float inDeltaTime) override;

private:
	SharedPtr<TextureResource> mPlayButtonTexture = nullptr;
	SharedPtr<TextureResource> mPauseButtonTexture = nullptr;
	SharedPtr<TextureResource> mStopButtonTexture = nullptr;

	Json mWorldJson;

	// Default to stopped. Switching to and from Stopped triggers saving or loading the world.
	ToolbarGameState mGameState = ToolbarGameState::Stopped;
};
