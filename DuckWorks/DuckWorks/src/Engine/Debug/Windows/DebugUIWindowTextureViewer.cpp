#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindowTextureViewer.h"

#include "External/imgui/imgui.h"
#include "External/SDL/SDL.h"

void DebugUIWindowTextureViewer::Update(float inDeltaTime)
{
	PROFILE_SCOPE(DebugUIWindowTextureViewer::Update)

	ImGui::Begin("Texture Viewer", &mOpen);

	if (mTexture == nullptr)
	{
		ImGui::Text("No texture selected");
		ImGui::End();
		return;
	}

	if (mTimeSinceClipboardCopied < mCopiedToClipboardTime)
	{
		ImGui::Text("Path copied to clipboard");
	}

	ImVec2 window_size = ImGui::GetContentRegionAvail();
	window_size.x -= 6; // Adjust for padding or borders if necessary
	window_size.y -= 6;

	// Get texture size
	float texture_width = static_cast<float>(mTexture->GetWidth());
	float texture_height = static_cast<float>(mTexture->GetHeight());

	// Calculate aspect ratios
	float texture_aspect_ratio = texture_width / texture_height;
	float window_aspect_ratio = window_size.x / window_size.y;

	ImVec2 displaySize;
	if (window_aspect_ratio > texture_aspect_ratio) // Window is wider than texture
	{
		displaySize.y = window_size.y;
		displaySize.x = displaySize.y * texture_aspect_ratio;
	}
	else // Window is narrower than texture
	{
		displaySize.x = window_size.x;
		displaySize.y = displaySize.x / texture_aspect_ratio;
	}

	if (ImGui::ImageButton("##TextureViewerButton", (ImTextureID)mTexture->mTexture, displaySize))
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
