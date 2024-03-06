#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindowTextureViewer.h"

#include "External/imgui/imgui.h"
#include "External/SDL/SDL.h"

void DebugUIWindowTextureViewer::Update(float inDeltaTime)
{
	ImGui::Begin("Texture Viewer", &mOpen);

	if (mTexture == nullptr)
	{
		ImGui::Text("No texture selected");
		ImGui::End();
		return;
	}

	if (mTimeSinceClipboardCopied < mCopiedToClipboardTime)
	{
		ImGui::Text("Copied to clipboard");
	}

	ImVec2 window_size = ImGui::GetContentRegionAvail();
	window_size.x -= 6;
	window_size.y -= 6;

	if (ImGui::ImageButton("##TextureViewerButton", (ImTextureID)mTexture->mTexture, window_size))
	{
		if (SDL_SetClipboardText(mTexture->GetFileName().c_str()) != 0)
			gLog(LogType::Error, "Unable to set clipboard text! SDL_Error: %s", SDL_GetError());

		mTimeSinceClipboardCopied = 0.f;
	}

	if (mTimeSinceClipboardCopied > 1000.f) // To prevent overflow
		mTimeSinceClipboardCopied = 1000.f;

	mTimeSinceClipboardCopied += inDeltaTime;

	ImGui::End();
}
