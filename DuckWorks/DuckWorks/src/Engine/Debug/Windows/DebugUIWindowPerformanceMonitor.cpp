#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindowPerformanceMonitor.h"

// External includes
#include <External/imgui/imgui.h>

RTTI_CLASS_DECLARATION(DebugUIWindowPerformanceMonitor)

RTTI_EMPTY_SERIALIZE_DEFINITION(DebugUIWindowPerformanceMonitor)

DebugUIWindowPerformanceMonitor::DebugUIWindowPerformanceMonitor()
{
	mTimings.resize(128);
}

void DebugUIWindowPerformanceMonitor::Update(float inDeltaTime)
{
	PROFILE_SCOPE(DebugUIWindowPerformanceMonitor::Update)

	float delta_time_seconds = inDeltaTime * 1000.f;

	ImGui::Begin("Performance Monitor", &mOpen, ImGuiWindowFlags_NoResize);

	mTimingsIndex = (mTimingsIndex + 1) % mTimings.size();
	mTimings[mTimingsIndex] = delta_time_seconds;

	if (delta_time_seconds > mHighestTiming)
		mHighestTiming = delta_time_seconds;

	float average_timing = 0.f;
	for (uint64 i = 0; i < mTimings.size(); i++)
	{
		average_timing += mTimings[i];
	}
	average_timing = average_timing / Cast<float>(mTimings.size());

	float fps = 1.f / average_timing * 1000.f;
	if (fps < 1.f)
		fps = 1.f;

	// When we're at the last index we can resize the array to fit the average fps
	if (mTimingsIndex == mTimings.size() - 1)
	{
		// Check if highest timing is ridiculously high, if so, reset it
		if (mHighestTiming > average_timing * 10)
		{
			gLog("Highest timing was %f, average timing was %f, resetting", mHighestTiming, average_timing);
			mHighestTiming = average_timing;
		}
		if ((SCast<float>(mTimings.size()) < fps * 0.9f || SCast<float>(mTimings.size()) > fps * 1.1f))
		{
			mTimings.resize(Cast<uint64>(fps));
		}
	}

	ImGui::Text("Frame time (ms): %.3f | FPS: %.0f", average_timing, fps);

	ImVec2 window_size = ImGui::GetWindowSize();

	//A line graph of the last 128 frame times
	ImGui::Text("Frame time graph: 0 - %.3f (ms)", mHighestTiming);
	ImGui::PlotLines("##FrameTimingsLineGraph", mTimings.data(), SCast<int>(mTimings.size()), 0, nullptr, FLT_EPSILON,
					mHighestTiming, ImVec2(window_size.x, 40));


	ImGui::End();
}
