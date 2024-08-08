#pragma once
#include "DebugUIWindow.h"

#ifdef _DEBUG

class DebugUIWindowMemoryDebugger : public DebugUIWindow
{
	RTTI_CLASS(DebugUIWindowMemoryDebugger, DebugUIWindow)

public:
	virtual void UpdateMultiThreaded(float inDeltaTime) override;
	virtual void Update(float inDeltaTime) override;

private:
	void DisplayMemory(const char* inTitle, uint64 inMemory);

	// Frame variables
	uint64 mTotalMemoryUsage = 0;
	uint64 mPeakMemoryUsage = 0;

	HashMap<String, uint64> mAllocatorMemory;

	HashMap<String, uint64> mResourceMemory;
};

#endif
