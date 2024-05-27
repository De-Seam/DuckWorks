#pragma once
#include "DebugUIWindow.h"

// Engine includes
#include "Engine/Resources/ResourceTypes/TextureResource.h"

class DebugUIWindowFileExplorer : public DebugUIWindow
{
	RTTI_CLASS(DebugUIWindowFileExplorer, DebugUIWindow, StandardAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	using Base::Base;

	DebugUIWindowFileExplorer(const ConstructParameters& inConstructParameters = {});

	virtual void UpdateMultiThreaded(float inDeltaTime) override;
	virtual void Update(float inDeltaTime) override;

	enum class FileType
	{
		Texture,
		Other
	};

private:
	SharedPtr<TextureResource> mFolderTexture = nullptr;
	SharedPtr<TextureResource> mFileTexture = nullptr;
	String mCurrentPath = "Assets";

	fm::vec2 mIconSize = {64.f, 64.f};

	HashMap<String, SharedPtr<TextureResource>> mTextures;

	// Per frame variables
	Array<String> mFoldersInCurrentDirectory;
	Array<std::filesystem::directory_entry> mDirectoryEntries;
	Array<std::filesystem::directory_entry> mFileEntries;

private:
	void UpdateEntry(const std::filesystem::directory_entry& inEntry, int32& ioButtonCount, int32 inButtonsPerRow);
	const Array<String>& ListFoldersInDirectory(const std::filesystem::path& inDirectoryPath);
	String TruncateTextToFitWidth(String& inText, float inMaxWidth);
};
