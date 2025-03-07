#pragma once
#include <DuckCore/Containers/Array.h>

#include <Editor/Editor.h>

#include <imgui/imgui.h>

class DuckEditor : public Editor
{
	RTTI_CLASS(DuckEditor, Editor)
public:
	DuckEditor();

	virtual void Update() override;

	void AddEditor(const DC::Ref<Editor>& aEditor);
	void RemoveEditor(const DC::Ref<Editor>& aEditor);

private:
	DC::Array<DC::Ref<Editor>> mEditors;

	ImGuiID mDockSpaceID = 0; // Dock space ID for main dock space.
};
