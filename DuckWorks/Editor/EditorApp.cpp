#include <Editor/EditorApp.h>

using namespace DC;

EditorApp::EditorApp()
{
	mDuckEditor = new DuckEditor;
}

void EditorApp::Update(float aDeltaTime)
{
	mDuckEditor->Update();
}
