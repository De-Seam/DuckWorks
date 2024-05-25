#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindowPerformanceMonitor.h"

// External includes
#include <External/imgui/imgui.h>

RTTI_CLASS_DEFINITION(DebugUIWindowPerformanceMonitor)

RTTI_EMPTY_SERIALIZE_DEFINITION(DebugUIWindowPerformanceMonitor)

DebugUIWindowPerformanceMonitor::DebugUIWindowPerformanceMonitor(const ConstructParameters& inConstructParameters)
	: Base(inConstructParameters)
{
	mTimings.resize(128);
}

void DebugUIWindowPerformanceMonitor::UpdateMultiThreaded(float inDeltaTime)
{
	PROFILE_SCOPE(DebugUIWindowPerformanceMonitor::UpdateMultiThreaded)

	if (!mVisible)
		return;

	float delta_time_seconds = inDeltaTime * 1000.f;

	mTimingsIndex = (mTimingsIndex + 1) % mTimings.size();
	mTimings[mTimingsIndex] = delta_time_seconds;

	if (delta_time_seconds > mHighestTiming)
		mHighestTiming = delta_time_seconds;

	mAverageTiming = 0.f;
	for (uint64 i = 0; i < mTimings.size(); i++)
	{
		mAverageTiming += mTimings[i];
	}
	mAverageTiming = mAverageTiming / SCast<float>(mTimings.size());

	mFPS = 1.f / mAverageTiming * 1000.f;
	if (mFPS < 1.f)
		mFPS = 1.f;

	// When we're at the last index we can resize the array to fit the average fps
	if (mTimingsIndex == mTimings.size() - 1)
	{
		// Check if highest timing is ridiculously high, if so, reset it
		if (mHighestTiming > mAverageTiming * 10)
		{
			gLog("Highest timing was %f, average timing was %f, resetting", mHighestTiming, mAverageTiming);
			mHighestTiming = mAverageTiming;
		}
		if ((SCast<float>(mTimings.size()) < mFPS * 0.9f || SCast<float>(mTimings.size()) > mFPS * 1.1f))
		{
			mTimings.resize(SCast<uint64>(mFPS));
		}
	}
}

void DebugUIWindowPerformanceMonitor::Update(float)
{
	PROFILE_SCOPE(DebugUIWindowPerformanceMonitor::Update)

	if (!ImGui::Begin("Performance Monitor", &mOpen, ImGuiWindowFlags_NoResize))
	{
		mVisible = false;
		ImGui::End();
		return;
	}
	mVisible = true;

	ImGui::Text("Frame time (ms): %.3f | FPS: %.0f", mAverageTiming, mFPS);

	ImVec2 window_size = ImGui::GetWindowSize();

	//A line graph of the last 128 frame times
	ImGui::Text("Frame time graph: 0 - %.3f (ms)", mHighestTiming);
	ImGui::PlotLines("##FrameTimingsLineGraph", mTimings.data(), SCast<int>(mTimings.size()), 0, nullptr, FLT_EPSILON,
					mHighestTiming, ImVec2(window_size.x, 40));

	ImGui::End();
}
