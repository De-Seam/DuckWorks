#include <Editor/DuckEditor/DuckEditor.h>

#include <DuckCore/Managers/Managers.h>
#include <DuckCore/Math/Random.h>

#include <Engine/Renderer/Renderer.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

using namespace DC;

DuckEditor::DuckEditor()
{
	mDockSpaceID = gWangHash(512);
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

	ImGui::DockSpaceOverViewport(mDockSpaceID, ImGui::GetMainViewport());

	static bool open = false;
	ImGui::ShowDemoWindow(&open);

	// Now create the docked window that can't be undocked
	ImGui::Begin("Info##DuckEditorInfoWindow", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
	ImGui::DockBuilderDockWindow("Info##DuckEditorInfoWindow", mDockSpaceID);

	ImGui::Text("This window is docked and cannot be undocked.");

	ImGui::End();
}

void DuckEditor::AddEditor(const Ref<Editor>& aEditor)
{
	mEditors.Add(aEditor);
}

void DuckEditor::RemoveEditor(const Ref<Editor>& aEditor)
{
	gVerify(mEditors.SwapRemoveFirstIf([&aEditor](const Editor* aItem) { return aEditor == aItem; }));
}
