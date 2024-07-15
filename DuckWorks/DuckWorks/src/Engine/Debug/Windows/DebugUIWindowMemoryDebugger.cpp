#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindowMemoryDebugger.h"

#ifdef _DEBUG

// Core includes
#include "Core/Utilities/PlatformFunctions.h"

// External includes
#include "External/imgui/imgui.h"

RTTI_CLASS_DEFINITION(DebugUIWindowMemoryDebugger, StandardAllocator)
RTTI_EMPTY_SERIALIZE_DEFINITION(DebugUIWindowMemoryDebugger)

DebugUIWindowMemoryDebugger::DebugUIWindowMemoryDebugger(const ConstructParameters& inConstructParameters)
	: Base(inConstructParameters) {}

void DebugUIWindowMemoryDebugger::UpdateMultiThreaded(float)
{
	PROFILE_SCOPE(DebugUIWindowMemoryDebugger::UpdateMultiThreaded)

	mTotalMemoryUsage = gGetMemoryUsage();
	mMaxMemoryUsage = gMax(mMaxMemoryUsage, mTotalMemoryUsage);
}

void DebugUIWindowMemoryDebugger::Update(float)
{
	PROFILE_SCOPE(DebugUIWindowMemoryDebugger::Update)

	if (!ImGui::Begin("Memory Debugger", &mOpen))
	{
		ImGui::End();
		return;
	}

	DisplayMemory("Total Memory Usage:", mTotalMemoryUsage);
	ImGui::Separator();
	DisplayMemory("Max Memory Usage:", mMaxMemoryUsage);
	ImGui::Separator();

#ifdef TRACK_ALLOCATIONS

	for (AllocatorBase* allocator : (*gAllocators))
	{
		HashMap<void*, AllocatorBase::AllocationData>& allocations = allocator->GetAllocations();
		uint64 total_allocated_size = 0;
		for (Pair<void* const, AllocatorBase::AllocationData>& pair : allocations)
		{
			total_allocated_size += pair.second.mSize;
		}
		DisplayMemory("Allocated Memory:", total_allocated_size);
		ImGui::Separator();
	}

#endif // TRACK_ALLOCATIONS

	ImGui::End();
}

void DebugUIWindowMemoryDebugger::DisplayMemory(const char* inTitle, uint64 inMemory)
{
	ImGui::Text(inTitle);
	ImGui::Text("%d Bytes", inMemory);
	double memory = SCast<double>(inMemory);
	double memory_kb = memory / 1024;
	if (memory_kb < 1)
		return;
	ImGui::Text("%.1f KiloBytes", memory_kb);
	double memory_mb = memory_kb / 1024;
	if (memory_mb < 1)
		return;
	ImGui::Text("%.1f MegaBytes", memory_mb);
	double memory_gb = memory_mb / 1024;
	if (memory_gb < 1)
		return;
	ImGui::Text("%.1f GigaBytes", memory_gb);
}

#endif // _Debug
