#pragma once
#include "DebugUIWindow.h"

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
};
