#pragma once
#include "DebugUIWindow.h"

#ifdef _DEBUG

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

#endif
