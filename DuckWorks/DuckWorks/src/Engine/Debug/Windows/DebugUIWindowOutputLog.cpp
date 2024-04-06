#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindowOutputLog.h"

#include "External/imgui/imgui.h"

RTTI_CLASS_DEFINITION(DebugUIWindowOutputLog)

Json DebugUIWindowOutputLog::Serialize() const
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mShowInfo);
	JSON_SAVE(json, mShowWarnings);
	JSON_SAVE(json, mShowErrors);
	JSON_SAVE(json, mMaxEntryCount);

	return json;
}

void DebugUIWindowOutputLog::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	JSON_LOAD(inJson, mShowInfo);
	JSON_LOAD(inJson, mShowWarnings);
	JSON_LOAD(inJson, mShowErrors);
	JSON_LOAD(inJson, mMaxEntryCount);
}

void DebugUIWindowOutputLog::Update(float)
{
	PROFILE_SCOPE(DebugUIWindowOutputLog::Update)

	ImGui::Begin("Output Log", &mOpen);

	ImGui::Checkbox("Info##DebugUIWindowOutputLogFilter", &mShowInfo);
	ImGui::SameLine();
	ImGui::Checkbox("Warning##DebugUIWindowOutputLogFilter", &mShowWarnings);
	ImGui::SameLine();
	ImGui::Checkbox("Error##DebugUIWindowOutputLogFilter", &mShowErrors);
	ImGui::SameLine();
	ImGui::SetNextItemWidth(64.f);
	ImGui::InputInt("Max Entries", &mMaxEntryCount, 0, 0);
	fm::clamp(mMaxEntryCount, 10, INT32_MAX);

	if (ImGui::BeginChild("LogContent", ImVec2(0, 0)))
	{
		gLogManager.GetLogMutex().ReadLock();
		const Array<LogManager::LogEntry>& log_array = gLogManager.GetLogArray();

		// Check if the user has scrolled up
		if (ImGui::GetScrollY() < ImGui::GetScrollMaxY())
			mAutoScroll = false;

		int32 start_index = log_array.size() - mMaxEntryCount;
		start_index = fm::max(start_index, 0);
		for (uint64 i = start_index; i < log_array.size(); i++)
		{
			const LogManager::LogEntry& log_entry = log_array[i];
			switch (log_entry.mType)
			{
			case LogType::Info:
				if (mShowInfo)
					ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.f), log_entry.mMessage.c_str());
				break;
			case LogType::Warning:
				if (mShowWarnings)
					ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), log_entry.mMessage.c_str());
				break;
			case LogType::Error:
				if (mShowErrors)
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

		ImGui::EndChild();
	}

	ImGui::End();
}
