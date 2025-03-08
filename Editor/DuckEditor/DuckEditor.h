#pragma once
#include <DuckCore/Containers/Array.h>

#include <Editor/Editor.h>

#include <imgui/imgui.h>

class DuckEditor : public Editor
{
	RTTI_CLASS(DuckEditor, Editor)
public:
	DuckEditor();

	virtual void OnFirstUpdate() override;
	virtual void Update() override;

	void AddEditor(DC::Ref<Editor> aEditor);
	void RemoveEditor(DC::Ref<Editor> aEditor);

	ImGuiID GetDockSpaceID() const { return mDockSpaceID; }

private:
	DC::Array<DC::Ref<Editor>> mEditors;

	ImGuiID mDockSpaceID = 0; // Dock space ID for main dock space.

	bool mHasBeenUpdated = false;
};
