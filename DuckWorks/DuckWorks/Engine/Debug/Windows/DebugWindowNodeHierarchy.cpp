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

	//Json json = root_node.Serialize();
	//if (DebugManager::sDebugDrawJson(json, "Root Node"))
	//	root_node.Deserialize(json);

	Node& root_node = gEngine->GetWorld()->GetRootNode();
	for (Node* child : root_node.GetChildren())
		RecursiveDrawNode(*child, 0);

	ImGui::End();
}

void DebugWindowNodeHierarchy::RecursiveDrawNode(Node& inNode, int inDepth)
{
	if (!mWasCollapsedMap.contains(inNode.GetGUID()))
		mWasCollapsedMap[inNode.GetGUID()] = true;

	const char* class_name = inNode.GetRTTI().GetClassName();
	const Array<Ref<Node>>& children = inNode.GetChildren();
	String label = String(class_name) + "##" + inNode.GetGUID().ToString();

	const bool is_selected = mSelectedNode.IsAlive() && mSelectedNode.Get()->GetGUID() == inNode.GetGUID();

	if (false)
	{
		if (is_selected)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.5f, 0.5f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.8f, 0.8f, 1.0f));
		}

		// Save the current style
		ImGuiStyle& style = ImGui::GetStyle();

		// Backup current style values
		ImVec4 color = style.Colors[ImGuiCol_Header];
		ImVec4 hoveredColor = style.Colors[ImGuiCol_HeaderHovered];
		ImVec4 activeColor = style.Colors[ImGuiCol_HeaderActive];
		float rounding = style.FrameRounding;
		ImVec2 padding = style.FramePadding;

		// Set style to match CollapsingHeader
		ImGui::PushStyleColor(ImGuiCol_Button, color);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoveredColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeColor);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);

		// Reduce left and right padding to match CollapsingHeader
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, padding.y));

		// Offset the button to remove the indentation
		float fullWidth = ImGui::GetContentRegionAvail().x + padding.x * 2;

		if (ImGui::Button(*label, ImVec2(fullWidth, 0.0f)))
			mSelectedNode = &inNode;

		// Restore the original style
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);

		if (is_selected)
			ImGui::PopStyleColor(3);
	}
	else
	{
		const bool was_collapsed = mWasCollapsedMap[inNode.GetGUID()];

		if (is_selected)
		{
			ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(1.0f, 0.5f, 0.5f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(1.0f, 0.8f, 0.8f, 1.0f));
		}
		ImGuiTreeNodeFlags flags = 0;
		if (inNode.GetChildren().empty())
			flags = ImGuiTreeNodeFlags_Bullet;
		if (ImGui::CollapsingHeader(*label, flags))
		{
			if (is_selected)
				ImGui::PopStyleColor(3);

			if (!was_collapsed)
			{
				mSelectedNode = &inNode;
				mWasCollapsedMap[inNode.GetGUID()] = true;
			}

			float indentation = 14.0f;
			ImGui::Indent(indentation);

			for (Node* child : children)
				RecursiveDrawNode(*child, inDepth + 1);

			ImGui::Unindent(indentation);
		}
		else
		{
			if (is_selected)
				ImGui::PopStyleColor(3);

			if (was_collapsed)
				mWasCollapsedMap[inNode.GetGUID()] = false;
		}
	}
}
