#pragma once
#include "DebugUIWindow.h"

// Engine includes
#include "Engine/Resources/ResourceTypes/TextureResource.h"

class DebugUIWindowFileExplorer : public DebugUIWindow
{
	RTTI_CLASS(DebugUIWindowFileExplorer, DebugUIWindow)

public:
	DebugUIWindowFileExplorer();

	virtual void Update(float inDeltaTime) override;

private:
	SharedPtr<TextureResource> mFolderTexture = nullptr;
	SharedPtr<TextureResource> mFileTexture = nullptr;
	String mCurrentPath = "Assets";

	fm::vec2 mIconSize = {64.f, 64.f};

	HashMap<String, SharedPtr<TextureResource>> mTextures;

private:
	void UpdateEntry(const std::filesystem::directory_entry& inEntry, int32& ioButtonCount, int32 inButtonsPerRow);
	Array<String> ListFolders(const std::filesystem::path& inDirectoryPath);
	String TruncateTextToFitWidth(String& inText, float inMaxWidth);
};
