#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindowOutputLog.h"

#include "External/imgui/imgui.h"

RTTI_CLASS_DEFINITION(DebugUIWindowOutputLog, StandardAllocator)

Json DebugUIWindowOutputLog::Serialize()
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

	JSON_TRY_LOAD(inJson, mShowInfo);
	JSON_TRY_LOAD(inJson, mShowWarnings);
	JSON_TRY_LOAD(inJson, mShowErrors);
	JSON_TRY_LOAD(inJson, mMaxEntryCount);
}

void DebugUIWindowOutputLog::Update(float)
{
	PROFILE_SCOPE(DebugUIWindowOutputLog::Update)

	if (!ImGui::Begin("Output Log", &mOpen))
	{
		ImGui::End();
		return;
	}

	ImGui::Checkbox("Info##DebugUIWindowOutputLogFilter", &mShowInfo);
	ImGui::SameLine();
	ImGui::Checkbox("Warning##DebugUIWindowOutputLogFilter", &mShowWarnings);
	ImGui::SameLine();
	ImGui::Checkbox("Error##DebugUIWindowOutputLogFilter", &mShowErrors);
	ImGui::SameLine();
	ImGui::SetNextItemWidth(64.f);
	ImGui::InputInt("Max Entries", &mMaxEntryCount, 0, 0);
	gClamp(mMaxEntryCount, 10, INT32_MAX);

	if (ImGui::BeginChild("LogContent", ImVec2(0, 0)))
	{
		const MutexReadProtectedPtr<Array<LogManager::LogEntry>> log_array = gLogManager.GetLogArray();

		// Check if the user has scrolled up
		if (ImGui::GetScrollY() < ImGui::GetScrollMaxY())
			mAutoScroll = false;

		int32 start_index = SCast<int32>(log_array->size()) - mMaxEntryCount;
		start_index = gMax(start_index, 0);
		for (uint64 i = SCast<uint64>(start_index); i < log_array->size(); i++)
		{
			const LogManager::LogEntry& log_entry = (*log_array.Get())[i];
			switch (log_entry.mType)
			{
			case ELogType::Info:
				if (mShowInfo)
					ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.f), log_entry.mMessage.c_str());
				break;
			case ELogType::Warning:
				if (mShowWarnings)
					ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), log_entry.mMessage.c_str());
				break;
			case ELogType::Error:
				if (mShowErrors)
					ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), log_entry.mMessage.c_str());
				break;
			}
		}

		// If the user scrolls back to the bottom manually, re-enable auto-scrolling
		if (!mAutoScroll && ImGui::GetScrollY() == ImGui::GetScrollMaxY())
			mAutoScroll = true;

		// Auto-scroll logic
		if (mAutoScroll)
			ImGui::SetScrollHereY(1.0f); // 1.0f is the bottom
	}
	ImGui::EndChild();

	ImGui::End();
}
