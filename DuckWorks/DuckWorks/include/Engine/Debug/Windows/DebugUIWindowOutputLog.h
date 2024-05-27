#pragma once
#include "DebugUIWindow.h"

// Engine includes
#include "Engine/Resources/ResourceTypes/TextureResource.h"

class DebugUIWindowOutputLog : public DebugUIWindow
{
	RTTI_CLASS(DebugUIWindowOutputLog, DebugUIWindow, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	using Base::Base;

	DebugUIWindowOutputLog(const ConstructParameters& inConstructParameters = {}) : Base(inConstructParameters) {}

	virtual void Update(float inDeltaTime) override;

private:
	bool mShowInfo = true;
	bool mShowWarnings = true;
	bool mShowErrors = true;

	bool mAutoScroll = true;

	int32 mMaxEntryCount = 512;

private:
};
