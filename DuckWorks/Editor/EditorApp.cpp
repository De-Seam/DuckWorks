#include <Editor/EditorApp.h>

void EditorApp::Init()
{
	mGameApp = DC::gMakeUnique<GameApp>();
	mGameApp->Init();
}

void EditorApp::Shutdown()
{
	mGameApp->Shutdown();
}

void EditorApp::Update(float inDeltaTime)
{
	mGameApp->Update(inDeltaTime);
}
