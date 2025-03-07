#include <Editor/DuckEditor/DuckEditor.h>

#include <DuckCore/Managers/Managers.h>

#include <Engine/Renderer/Renderer.h>

#include <imgui/imgui.h>

using namespace DC;

DuckEditor::DuckEditor()
{
}

/*
Update phase:
1. Update editor that's currently open.
2. Update next editor in update queue, so we update one editor each update, besides the open editor.
*/
void DuckEditor::Update()
{
	Base::Update();

	Renderer::ScopedRenderTarget scoped_render_target(*mRenderTarget);

	static bool open = false;
	ImGui::ShowDemoWindow(&open);

	/*
	ImGuiIO& io = ImGui::GetIO(); // Accessing ImGui IO
    ImVec2 windowSize = ImVec2(io.DisplaySize.x, io.DisplaySize.y); // Set the window size to the display size

    // Create a fullscreen ImGui window with no title, no border, and no resize/move functionality
    ImGui::SetNextWindowSize(windowSize);
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));  // Position at top-left corner

    ImGui::Begin("FullScreen Window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);
    
	if (ImGui::BeginTabBar("TabBar", ImGuiTabBarFlags_Reorderable))
	{
		bool item_open = true;
		bool open = ImGui::BeginTabItem("Test", &item_open, 0);
		if (open)
		{
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

    ImGui::End();  // End the ImGui window

	ImGui::Begin("DuckEditor");


	ImGui::End();
	*/
}

void DuckEditor::AddEditor(Ref<Editor> aEditor)
{
	mEditors.Add(aEditor);
}

void DuckEditor::RemoveEditor(Ref<Editor> aEditor)
{
	gVerify(mEditors.SwapRemoveFirstIf([&aEditor](const Editor* aItem) { return aEditor == aItem; }));
}
