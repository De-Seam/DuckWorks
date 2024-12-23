#include <DuckCore/Math/Transform.h>
#include <Editor/EditorApp.h>

EditorApp::EditorApp()
{
	mGameRenderTarget = gEngine->GetManager<Renderer>().CreateTexture(DC::IVec2(800, 600));

	mGameApp = DC::gMakeUnique<GameApp>();
}

void EditorApp::Update(float inDeltaTime)
{
	{
		// Render the game to the game render target
		Renderer::ScopedRenderTarget scoped_render_target(mGameRenderTarget);
		mGameApp->Update(inDeltaTime);
	}
	DC::Transform2D transform;
	transform.mHalfSize = DC::FVec2(800, 600) / 2.0f;
	gEngine->GetManager<Renderer>().DrawTexture(mGameRenderTarget, transform);
}
