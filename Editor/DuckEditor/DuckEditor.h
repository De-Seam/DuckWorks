#pragma once
#include <DuckCore/Containers/Array.h>

#include <Editor/Editor.h>

class DuckEditor : public Editor
{
	RTTI_CLASS(DuckEditor, Editor)
public:
	DuckEditor();

	virtual void Update() override;

	void AddEditor(DC::Ref<Editor> aEditor);
	void RemoveEditor(DC::Ref<Editor> aEditor);

private:
	DC::Array<DC::Ref<Editor>> mEditors;
};