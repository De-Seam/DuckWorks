#pragma once
#include <Editor/DuckEditor/DuckEditor.h>

#include <DuckWorks/App/App.h>

class EditorApp : public App
{
public:
	EditorApp();
	virtual void Update(float aDeltaTime) override;

private:
	DC::Ref<DuckEditor> mDuckEditor;
};
