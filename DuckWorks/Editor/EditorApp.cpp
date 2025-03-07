#include <DuckWorks/Editor/EditorApp.h>

#include <Game/Editor/GameEditor/GameEditor.h>

using namespace DC;

EditorApp::EditorApp()
{
	mDuckEditor = new DuckEditor;

	mDuckEditor->AddEditor(new GameEditor);
}

void EditorApp::Update(float aDeltaTime)
{
	mDuckEditor->Update();
}
