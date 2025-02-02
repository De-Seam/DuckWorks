#pragma once
#include "DebugUIWindow.h"

class DebugUIWindowPerformanceMonitor : public DebugUIWindow
{
	RTTI_CLASS(DebugUIWindowPerformanceMonitor, DebugUIWindow, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	using Base::Base;

	DebugUIWindowPerformanceMonitor(const ConstructParameters& inConstructParameters = {});

	DebugUIWindowPerformanceMonitor();

	virtual void UpdateMultiThreaded(float inDeltaTime) override;
	virtual void Update(float inDeltaTime) override;

private:
	std::vector<float> mTimings;
	uint64 mTimingsIndex = UINT64_MAX;
	float mHighestTiming = 0.f;
	bool mVisible = true;

	// Per frame calculated
	float mAverageTiming = 0.f;
	float mFPS = 0.f;
};
