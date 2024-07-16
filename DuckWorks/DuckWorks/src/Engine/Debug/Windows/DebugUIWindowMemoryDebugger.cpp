#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindowMemoryDebugger.h"

#ifdef _DEBUG

// Core includes
#include "Core/Utilities/PlatformFunctions.h"

// Engine includes
#include "Engine/Resources/ResourceManager.h"
#include "Engine/Resources/ResourceTypes/TextureResource.h"
#include "Engine/Resources/ResourceTypes/LuaResource.h"

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

	mAllocatorMemory.clear();
	std::ranges::sort(*gAllocators, [](AllocatorBase* a, AllocatorBase* b) { return a->GetAllocations().size() > b->GetAllocations().size(); });
	for (AllocatorBase* allocator : (*gAllocators))
	{
		HashMap<void*, AllocatorBase::AllocationData>& allocations = allocator->GetAllocations();
		uint64 total_allocated_size = 0;
		for (Pair<void* const, AllocatorBase::AllocationData>& pair : allocations)
			total_allocated_size += pair.second.mSize;
		mAllocatorMemory[allocator->GetName()] = total_allocated_size;
	}

	mResourceMemory.clear();
	const HashMap<String, SharedPtr<BaseResource>>& resources = gResourceManager.GetAllResources();
	for (const Pair<String, SharedPtr<BaseResource>>& pair : resources)
	{
		mResourceMemory[pair.second->GetClassName()] += pair.second->GetMemorySize();
	}
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

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0.f, 8.f});
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{0.f, 3.f});
	if (ImGui::TreeNodeEx("Allocators##TreeNode"))
	{
		ImGui::PopStyleVar(2);

		for (const Pair<String, uint64>& pair : mAllocatorMemory)
		{
			DisplayMemory(pair.first.c_str(), pair.second);
			ImGui::Separator();
		}

		ImGui::TreePop();
	}
	else
		ImGui::PopStyleVar(2);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0.f, 8.f});
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{0.f, 3.f});
	if (ImGui::TreeNodeEx("Resources##TreeNode"))
	{
		ImGui::PopStyleVar(2);

		for (const Pair<String, uint64>& pair : mResourceMemory)
		{
			DisplayMemory(pair.first.c_str(), pair.second);
			ImGui::Separator();
		}

		ImGui::TreePop();
	}
	else
		ImGui::PopStyleVar(2);

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
	ImGui::Text("%.1f KB", memory_kb);
	double memory_mb = memory_kb / 1024;
	if (memory_mb < 1)
		return;
	ImGui::Text("%.1f MB", memory_mb);
	double memory_gb = memory_mb / 1024;
	if (memory_gb < 1)
		return;
	ImGui::Text("%.1f GB", memory_gb);
}

#endif // _Debug
