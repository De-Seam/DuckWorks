#pragma once
#include "DebugUIWindow.h"

class DebugUIWindowNewFilePopup : public DebugUIWindow
{
	RTTI_CLASS(DebugUIWindowNewFilePopup, DebugUIWindow)

public:
	DebugUIWindowNewFilePopup() = default;

	virtual void UpdateMultiThreaded(float inDeltaTime) override;
	virtual void Update(float inDeltaTime) override;

	void SetPath(const String &inPath);

private:
	String mPath = "Assets/";
};
