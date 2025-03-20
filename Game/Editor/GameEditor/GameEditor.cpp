#include <Game/Editor/GameEditor/GameEditor.h>

#include <Renderer/Renderer.h>
#include <Editor/DuckEditor/DuckEditor.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

using namespace DC;

GameEditor::GameEditor() : Base(GUID::sCreate())
{
	mGame = new Game;

	mWindowLabel = String("Game Editor##") + GetGUID().ToString();
}

void GameEditor::OnFirstUpdate()
{
	Base::OnFirstUpdate();

	ImGuiID dock_space_id = GetDuckEditor()->GetDockSpaceID();
	ImGui::DockBuilderDockWindow(*mWindowLabel, dock_space_id);
	ImGui::DockBuilderFinish(dock_space_id);
}

void GameEditor::Update()
{
	Base::Update();

	{
		Renderer::ScopedRenderTarget scoped_render_target(*mRenderTarget);

		mGame->Update(GetDeltaTime());

		mGame->Render();
	}

	ImGui::Begin(*mWindowLabel);

	ImVec2 content_region_available = ImGui::GetContentRegionAvail();
	IVec2 current_size = GetSize();

	if (gStaticCast<int>(content_region_available.x) != current_size.mX || gStaticCast<int>(content_region_available.y) != current_size.mY)
		SetSize({ gStaticCast<int>(content_region_available.x), gStaticCast<int>(content_region_available.y) });

	ImGui::Image((ImTextureID)mRenderTarget->GetTexture(), { gStaticCast<float>(current_size.mX), gStaticCast<float>(current_size.mY) });

	ImGui::End();
}
