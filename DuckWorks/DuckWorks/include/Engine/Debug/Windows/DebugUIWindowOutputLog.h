#pragma once
#include "DebugUIWindow.h"

#ifdef _DEBUG

class DebugUIWindowOutputLog : public DebugUIWindow
{
	RTTI_CLASS(DebugUIWindowOutputLog, DebugUIWindow)

public:
	Json Serialize() const override;
	void Deserialize(const Json& inJson) override;

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
