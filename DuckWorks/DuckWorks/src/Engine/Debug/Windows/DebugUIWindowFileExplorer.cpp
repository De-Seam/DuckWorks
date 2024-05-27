#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindowFileExplorer.h"

// Engine includes
#include "Engine/Debug/DebugUIWindowManager.h"
#include "Engine/Debug/Windows/DebugUIWindowNewFilePopup.h"
#include "Engine/Debug/Windows/DebugUIWindowTextureViewer.h"
#include "Engine/Resources/ResourceManager.h"
#include "Engine/Resources/ResourceTypes/TextureResource.h"

// External includes
#include <External/imgui/imgui.h>

// Std includes
#include <algorithm>
#include <filesystem>

RTTI_CLASS_DEFINITION(DebugUIWindowFileExplorer, StandardAllocator)

Json DebugUIWindowFileExplorer::Serialize()
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mCurrentPath);
	JSON_SAVE(json, mIconSize);

	return json;
}

void DebugUIWindowFileExplorer::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	JSON_LOAD(inJson, mCurrentPath);
	JSON_LOAD(inJson, mIconSize);
}

DebugUIWindowFileExplorer::DebugUIWindowFileExplorer(const ConstructParameters& inConstructParameters)
	: Base(inConstructParameters)
{
	mFolderTexture = gResourceManager.GetResource<TextureResource>("Assets/Debug/Icon_Folder.png");
	mFileTexture = gResourceManager.GetResource<TextureResource>("Assets/Debug/Icon_File.png");
}

namespace fs = std::filesystem;

void DebugUIWindowFileExplorer::UpdateMultiThreaded(float)
{
	PROFILE_SCOPE(DebugUIWindowFileExplorer::UpdateMultiThreaded)

	mFoldersInCurrentDirectory = ListFoldersInDirectory(mCurrentPath);

	mDirectoryEntries.clear();
	mFileEntries.clear();
	for (const auto& entry : fs::directory_iterator(mCurrentPath))
	{
		if (entry.is_directory())
			mDirectoryEntries.push_back(entry);
		else if (entry.is_regular_file())
			mFileEntries.push_back(entry);
	}
}

void DebugUIWindowFileExplorer::Update(float)
{
	PROFILE_SCOPE(DebugUIWindowFileExplorer::Update)

	if (!ImGui::Begin("File Explorer", &mOpen))
	{
		ImGui::End();
		return;
	}

	fs::path directory_path = mCurrentPath;
	ImVec2 window_size = ImGui::GetWindowSize();

	for (const String& folder : mFoldersInCurrentDirectory)
	{
		if (ImGui::Button((folder + "##Path").c_str()))
		{
			// Replace the current path with the new path
			size_t start_pos = mCurrentPath.find(folder);
			if (start_pos != std::string::npos)
			{
				String new_path = "";
				for (const String& f : mFoldersInCurrentDirectory)
				{
					new_path += f;
					if (f == folder)
						break;
					new_path += "/";
				}
				mCurrentPath = new_path;
				directory_path = mCurrentPath;
			}
		}
		if (folder != mFoldersInCurrentDirectory.back())
		{
			ImGui::SameLine();
			ImGui::Text("/");
			ImGui::SameLine();
		}
	}

	// Calculate how many buttons fit in one row based on window width
	float window_width = ImGui::GetContentRegionAvail().x;
	int buttons_per_row = std::max(1, static_cast<int>(window_width / (mIconSize.x + ImGui::GetStyle().ItemSpacing.x)));
	int button_count = 0;

	try
	{
		if (exists(directory_path) && is_directory(directory_path))
		{
			// Back button with a label underneath (if not in the base "Assets" directory)
			if (mCurrentPath != "Assets")
			{
				ImGui::BeginGroup();
				if (ImGui::ImageButton((ImTextureID)mFolderTexture->mTexture, ImVec2(mIconSize.x, mIconSize.y)))
				{
					mCurrentPath = directory_path.parent_path().string();
					std::ranges::replace(mCurrentPath.begin(), mCurrentPath.end(), '\\', '/');
					gLog("Clicked on folder: %s", mCurrentPath.c_str());
				}
				ImGui::Text("Back"); // Label for the back button
				ImGui::EndGroup();

				if (++button_count < buttons_per_row)
					ImGui::SameLine();
				else
					button_count = 0; // Reset counter for the next row
			}

			// mDirectoryEntries and mFileEntries were populated multi threaded
			for (std::filesystem::directory_entry entry : mDirectoryEntries)
				UpdateEntry(entry, button_count, buttons_per_row);

			for (std::filesystem::directory_entry entry : mFileEntries)
				UpdateEntry(entry, button_count, buttons_per_row);
		}
	}
	catch (fs::filesystem_error& error)
	{
		gLog(ELogType::Error, error.what());
	}

	// Context menu (under default mouse threshold)
	ImGui::InvisibleButton("canvas", window_size, ImGuiButtonFlags_MouseButtonRight);
	ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
	if (drag_delta.x == 0.0f && drag_delta.y == 0.0f)
		ImGui::OpenPopupOnItemClick("Context", ImGuiPopupFlags_MouseButtonRight);
	if (ImGui::BeginPopup("Context"))
	{
		if (ImGui::MenuItem("New Folder", nullptr, false, true))
		{
			create_directory(directory_path / "NewFolder");
		}
		if (ImGui::MenuItem("New File", nullptr, false, true))
		{
			gDebugUIWindowManager.CreateWindow<DebugUIWindowNewFilePopup>();
		}
		ImGui::EndPopup();
	}

	ImGui::End();
}

void DebugUIWindowFileExplorer::UpdateEntry(const std::filesystem::directory_entry& inEntry, int32& ioButtonCount, int32 inButtonsPerRow)
{
	auto filename = inEntry.path().filename().string();

	ImGui::BeginGroup(); // Start grouping icon with its label

	String file_path = inEntry.path().string();
	std::ranges::replace(file_path.begin(), file_path.end(), '\\', '/');

	if (inEntry.is_directory())
	{
		if (ImGui::ImageButton(file_path.c_str(), static_cast<ImTextureID>(mFolderTexture->mTexture), ImVec2(mIconSize.x, mIconSize.y)))
		{
			mCurrentPath = file_path;
			gLog("Clicked on folder: %s", mCurrentPath.c_str());
		}
	}
	else if (inEntry.is_regular_file())
	{
		SharedPtr<TextureResource> texture;
		FileType file_type = FileType::Other;
		if (gIsValidTextureExtension(file_path))
		{
			texture = gResourceManager.GetResource<TextureResource>(file_path);
			file_type = FileType::Texture;
		}
		else
			texture = mFileTexture;

		mTextures[file_path] = texture;
		if (ImGui::ImageButton(file_path.c_str(), static_cast<ImTextureID>(texture->mTexture), ImVec2(mIconSize.x, mIconSize.y)))
		{
			gLog("Clicked on file: %s", file_path.c_str());

			switch (file_type)
			{
			case FileType::Texture:
			{
				if (!gDebugUIWindowManager.WindowExists<DebugUIWindowTextureViewer>())
					gDebugUIWindowManager.CreateWindow<DebugUIWindowTextureViewer>();

				Ref<DebugUIWindowTextureViewer> texture_viewer = gDebugUIWindowManager.GetWindow<DebugUIWindowTextureViewer>();

				texture_viewer->SetTexture(texture);
			}
			break;
			case FileType::Other:
			{
				gLog(ELogType::Warning, "Tried to open file which extension was not recognized: %s", file_path.c_str());
				break;
			}
			}
		}
	}

	String file_name_truncated = TruncateTextToFitWidth(filename, mIconSize.x);
	ImGui::Text("%s", file_name_truncated.c_str()); // Label for the directory

	ImGui::EndGroup(); // End grouping

	// Logic to determine whether to continue on the same line or wrap
	if (++ioButtonCount < inButtonsPerRow)
		ImGui::SameLine();
	else
		ioButtonCount = 0; // Reset counter for the next row
}

const Array<String>& DebugUIWindowFileExplorer::ListFoldersInDirectory(const fs::path& inDirectoryPath)
{
	static Array<String> folders;
	folders.clear();
	fs::path currentPath = inDirectoryPath;

	// Print or use the parent path
	folders.emplace_back(currentPath.filename().string());

	while (currentPath.has_parent_path())
	{
		// Move to the parent path
		currentPath = currentPath.parent_path();

		folders.emplace_back(currentPath.filename().string());
	}

	std::ranges::reverse(folders.begin(), folders.end());
	return folders;
}

String DebugUIWindowFileExplorer::TruncateTextToFitWidth(String& inText, float inMaxWidth)
{
	// Calculate the width of the text
	ImVec2 text_size = ImGui::CalcTextSize(inText.c_str());

	// If the text fits within the maxWidth, return it as is
	if (text_size.x <= inMaxWidth)
		return inText;

	// Start with a conservative estimate of how much of the text will fit
	size_t approximateCharsToFit = static_cast<size_t>((inMaxWidth / text_size.x) * inText.length());
	std::string truncatedText = inText.substr(0, approximateCharsToFit);

	// Refine the estimate by checking the size and adjusting the substring length
	while (!truncatedText.empty() && ImGui::CalcTextSize((truncatedText + ".").c_str()).x > inMaxWidth)
	{
		truncatedText.pop_back(); // Remove one character at a time
	}

	return truncatedText + "..";
}
