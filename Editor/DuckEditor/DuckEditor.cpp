#include <Editor/DuckEditor/DuckEditor.h>

#include <DuckCore/Managers/Managers.h>
#include <DuckCore/Math/Random.h>

#include <Renderer/Renderer.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

using namespace DC;

DuckEditor::DuckEditor()
{
	mDockSpaceID = Hash("Info##DuckEditorInfoWindow");
}

void DuckEditor::OnFirstUpdate()
{
	Base::OnFirstUpdate();

	ImGui::DockBuilderDockWindow("Info##DuckEditorInfoWindow", mDockSpaceID);
	ImGui::DockBuilderFinish(mDockSpaceID);

	for (Editor* editor : mEditors)
		editor->OnFirstUpdate();
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

	if (!mHasBeenUpdated)
	{
		OnFirstUpdate();
		mHasBeenUpdated = true;
	}

	ImGui::DockSpaceOverViewport(mDockSpaceID, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoDockingSplit);

	static bool open = false;
	ImGui::ShowDemoWindow(&open);

	// Now create the docked window that can't be undocked
	ImGui::Begin("Info##DuckEditorInfoWindow", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

	ImGui::Text("This window is docked and cannot be undocked.");

	ImGui::End();

	for (Editor* editor : mEditors)
		editor->Update();
}

void DuckEditor::AddEditor(Ref<Editor> aEditor)
{
	mEditors.Add(Move(aEditor));
	mEditors.Back()->OnAddedToDuckEditor(*this);
}

void DuckEditor::RemoveEditor(Ref<Editor> aEditor)
{
	gVerify(mEditors.SwapRemoveFirstIf([&aEditor](const Editor* aItem) { return aEditor == aItem; }));
	aEditor->OnRemovedFromDuckEditor(*this);
}
