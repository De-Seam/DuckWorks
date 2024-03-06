#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindowOutputLog.h"

#include "External/imgui/imgui.h"

void DebugUIWindowOutputLog::Update(float inDeltaTime)
{
	ImGui::Begin("Output Log", &mOpen);

	ImGui::Checkbox("Info##DebugUIWindowOutputLogFilter", &mShowInfo);
	ImGui::SameLine();
	ImGui::Checkbox("Warning##DebugUIWindowOutputLogFilter", &mShowWarnings);
	ImGui::SameLine();
	ImGui::Checkbox("Error##DebugUIWindowOutputLogFilter", &mShowErrors);

	gLogManager.GetLogMutex().ReadLock();
	const Array<LogManager::LogEntry>& log_array = gLogManager.GetLogArray();

	// Check if the user has scrolled up
	if (ImGui::GetScrollY() < ImGui::GetScrollMaxY())
		mAutoScroll = false;

	for (const LogManager::LogEntry& log_entry : log_array)
	{
		if (log_entry.mType == LogType::Info && !mShowInfo)
			continue;
		if (log_entry.mType == LogType::Warning && !mShowWarnings)
			continue;
		if (log_entry.mType == LogType::Error && !mShowErrors)
			continue;

		switch (log_entry.mType)
		{
		case LogType::Info:
			ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.f), log_entry.mMessage.c_str());
			break;
		case LogType::Warning:
			ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), log_entry.mMessage.c_str());
			break;
		case LogType::Error:
			ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), log_entry.mMessage.c_str());
			break;
		}
	}
	gLogManager.GetLogMutex().ReadUnlock();

	// If the user scrolls back to the bottom manually, re-enable auto-scrolling
	if (!mAutoScroll && ImGui::GetScrollY() == ImGui::GetScrollMaxY())
		mAutoScroll = true;

	// Auto-scroll logic
	if (mAutoScroll)
		ImGui::SetScrollHereY(1.0f); // 1.0f is the bottom

	ImGui::End();
}
