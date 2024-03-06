#pragma once
#include "DebugUIWindow.h"

// Engine includes
#include "Engine/Resources/ResourceTypes/TextureResource.h"

class DebugUIWindowOutputLog : public DebugUIWindow
{
	RTTI_CLASS(DebugUIWindowOutputLog, DebugUIWindow)

public:
	virtual void Update(float inDeltaTime) override;

private:
	bool mShowInfo = true;
	bool mShowWarnings = true;
	bool mShowErrors = true;

	bool mAutoScroll = true;

private:
};
