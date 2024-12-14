#include <Editor/EditorApp.h>

EditorApp::EditorApp()
{
	mGameApp = DC::gMakeUnique<GameApp>();
}

void EditorApp::Update(float inDeltaTime)
{
	mGameApp->Update(inDeltaTime);
}
