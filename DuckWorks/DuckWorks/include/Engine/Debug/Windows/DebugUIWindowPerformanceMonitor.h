#pragma once
#include "DebugUIWindow.h"

class DebugUIWindowPerformanceMonitor : public DebugUIWindow
{
	RTTI_CLASS(DebugUIWindowPerformanceMonitor, DebugUIWindow);

public:
	DebugUIWindowPerformanceMonitor();

	virtual void Update(float inDeltaTime) override;

private:
	std::vector<float> mTimings;
	uint64 mTimingsIndex = UINT64_MAX;
	float mHighestTiming = 0.f;
};
