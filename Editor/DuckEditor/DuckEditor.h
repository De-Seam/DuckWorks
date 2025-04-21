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

	template <typename taType, typename... taArgs>
	void CreateEditor(taArgs&&... aArgs);
	void RemoveEditor(const Editor& aEditor) { RemoveEditor(aEditor.GetGUID()); }
	void RemoveEditor(const DC::GUID& aGUID);

	ImGuiID GetDockSpaceID() const { return mDockSpaceID; }

private:
	DC::Array<DC::Ref<Editor>> mEditors;

	ImGuiID mDockSpaceID = 0; // Dock space ID for main dock space.

	bool mHasBeenUpdated = false;
};

template<typename taType, typename ... taArgs>
void DuckEditor::CreateEditor(taArgs&&... aArgs)
{
	mEditors.Add(new taType(std::forward<taArgs>(aArgs)...));
}
