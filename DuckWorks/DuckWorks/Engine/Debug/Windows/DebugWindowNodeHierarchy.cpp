#include <Precomp.h>
#include <Engine/Debug/Windows/DebugWindowNodeHierarchy.h>

// Engine includes
#include <Engine/Debug/DebugManager.h>
#include <Engine/Engine.h>

// External includes
#include <External/imgui/imgui.h>

void DebugWindowNodeHierarchy::Update(float inDeltaTime)
{
	PROFILE_SCOPE(DebugWindowNodeHierarchy::Update)
	ImGui::Begin("Node Hierarchy", &mIsOpen);

	Node& root_node = gEngine->GetWorld()->GetRootNode();
	Json json = root_node.Serialize();
	if (DebugManager::sDebugDrawJson(json, "Root Node"))
		root_node.Deserialize(json);

	ImGui::End();
}
