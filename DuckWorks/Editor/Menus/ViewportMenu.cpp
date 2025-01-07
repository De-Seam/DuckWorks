#include <DuckCore/Math/Transform.h>

#include <Editor/Menus/ViewportMenu.h>

#include <Engine/Renderer/Renderer.h>

#include <imgui/imgui.h>

using namespace DC;

ViewportMenu::ViewportMenu(EditorApp& inEditorApp) :
	Base(inEditorApp)
{
	mGameRenderTarget = gEngine->GetManager<Renderer>().CreateTexture(IVec2(800, 600));

	mGameApp = gMakeUnique<GameApp>();
}

void ViewportMenu::Update(float inDeltaTime)
{
	if (!mIsOpen)
		return;

	ImGui::Begin("Viewport", &mIsOpen);

	{
		// Render the game to the game render target
		Renderer::ScopedRenderTarget scoped_render_target(mGameRenderTarget);
		mGameApp->Update(inDeltaTime);
	}

	ImGui::Image((ImTextureID)mGameRenderTarget, {800, 600});

	//Transform2D transform;
	//transform.mHalfSize = FVec2(800, 600) / 2.0f;
	//gEngine->GetManager<Renderer>().DrawTexture(mGameRenderTarget, transform);

	ImGui::End();
}
