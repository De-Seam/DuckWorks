#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindowEditorToolbar.h"

// Engine includes
#include "Engine/Resources/ResourceManager.h"
#include "Engine/Resources/ResourceTypes/TextureResource.h"
#include "Engine/World/World.h"

// External includes
#include <External/imgui/imgui.h>

// Game includes
#include "Game/App/App.h"

// Std includes
#include <fstream>

RTTI_CLASS_DEFINITION(DebugUIWindowEditorToolbar)

RTTI_EMPTY_SERIALIZE_DEFINITION(DebugUIWindowEditorToolbar)

DebugUIWindowEditorToolbar::DebugUIWindowEditorToolbar()
{
	mSaveButtonTexture = gResourceManager.GetResource<TextureResource>("Assets/Debug/Icon_SaveButton.png");
	mPlayButtonTexture = gResourceManager.GetResource<TextureResource>("Assets/Debug/Icon_PlayButton.png");
	mPauseButtonTexture = gResourceManager.GetResource<TextureResource>("Assets/Debug/Icon_PauseButton.png");
	mStopButtonTexture = gResourceManager.GetResource<TextureResource>("Assets/Debug/Icon_StopButton.png");

	gApp.SetPaused(true);
}

void DebugUIWindowEditorToolbar::UpdateMultiThreaded(float inDeltaTime) {}

void DebugUIWindowEditorToolbar::Update(float inDeltaTime)
{
	if (mWorldJson.empty() && mGameState == ToolbarGameState::Stopped)
		Save();

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {0, 0});
	if (!ImGui::Begin("Editor Toolbar", &mOpen, window_flags))
	{
		ImGui::PopStyleVar(2);
		ImGui::End();
		return;
	}

	ImGui::SetWindowSize({38 * 3, 32});

	if (ImGui::ImageButton("##SaveButton", (ImTextureID)mSaveButtonTexture->mTexture, {32, 32}))
		Save();

	ImGui::SameLine();

	bool paused = gApp.IsPaused();
	SharedPtr<TextureResource> texture = paused ? mPauseButtonTexture : mPlayButtonTexture;
	if (ImGui::ImageButton("##PausePlayButton", (ImTextureID)texture->mTexture, {32, 32}, {1, 0}, {0, 1}))
	{
		if (paused && mGameState == ToolbarGameState::Stopped)
			Save();


		mGameState = paused ? ToolbarGameState::Playing : ToolbarGameState::Paused;
		gApp.SetPaused(!paused);

		if (!gApp.GetWorld()->HasBegunPlay())
			gApp.GetWorld()->BeginPlay();
	}
	ImGui::SameLine();
	bool stop_button_enabled = mGameState != ToolbarGameState::Stopped;
	ImVec4 tint_color = stop_button_enabled ? ImVec4{1.f, 1.f, 1.f, 1.f} : ImVec4{0.5f, 0.5f, 0.5f, 1.f};
	if (ImGui::ImageButton("##StopButton", (ImTextureID)mStopButtonTexture->mTexture, {32, 32}, {0, 0}, {1, 1}, {0, 0, 0, 0}, tint_color) &&
		stop_button_enabled)
	{
		StopPlay();
	}

	ImGui::PopStyleVar(2);
	ImGui::End();
}

void DebugUIWindowEditorToolbar::StopPlay() 
{
	mGameState = ToolbarGameState::Stopped;
	gApp.CreateNewWorld(mWorldJson);
	gApp.SetPaused(true);
}

void DebugUIWindowEditorToolbar::Save() {
	mWorldJson = gApp.GetWorld()->Serialize();
}

void DebugUIWindowEditorToolbar::SaveStateToFile()
{
	std::ofstream file("world.json");
	file << mWorldJson.dump(4);
}
