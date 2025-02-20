#include <Editor/Menus/ViewportMenu.h>

#include <DuckCore/Manager/Managers.h>
#include <DuckCore/Math/Transform.h>

#include <Engine/Renderer/Renderer.h>

#include <imgui/imgui.h>

using namespace DC;

ViewportMenu::ViewportMenu(EditorApp& inEditorApp) :
	Base(inEditorApp)
{
	mGameRenderTarget = Managers::sGet<Renderer>().CreateTexture(IVec2(mGameRenderTargetSize.mX, mGameRenderTargetSize.mY));

	mGameApp = MakeUnique<GameApp>();
}

void ViewportMenu::Update(float inDeltaTime)
{
	if (!mIsOpen)
		return;

	ImGui::Begin("Viewport", &mIsOpen);

	ImVec2 region_available_flt = ImGui::GetContentRegionAvail();
	UVec2 region_available =
	{
		gStaticCast<uint>(region_available_flt.x),
		gStaticCast<uint>(region_available_flt.y)
	};
	if (region_available.mX != mGameRenderTargetSize.mX && region_available.mX != mGameRenderTargetSize.mY && 
		region_available.mX > 0 && region_available.mY > 0)
	{
		Renderer& renderer = Managers::sGet<Renderer>();;
		if (mGameRenderTarget != nullptr)
			renderer.DestroyTexture(mGameRenderTarget);
		mGameRenderTarget = renderer.CreateTexture(region_available.As<int>());
		mGameRenderTargetSize = region_available.As<uint>();
	}

	{
		// Render the game to the game render target
		Renderer::ScopedRenderTarget scoped_render_target(mGameRenderTarget);
		mGameApp->Update(inDeltaTime);
	}

	ImGui::Image((ImTextureID)mGameRenderTarget, {gStaticCast<float>(mGameRenderTargetSize.mX), gStaticCast<float>(mGameRenderTargetSize.mY)});

	ImGui::End();
}
