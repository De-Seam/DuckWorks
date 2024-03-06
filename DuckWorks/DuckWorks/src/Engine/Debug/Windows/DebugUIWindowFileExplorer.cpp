#include "Precomp.h"
#include "Engine/Debug/Windows/DebugUIWindowFileExplorer.h"

// Engine includes
#include "Engine/Resources/ResourceManager.h"
#include "Engine/Resources/ResourceTypes/TextureResource.h"
#include "Engine/Debug/DebugUIWindowManager.h"
#include "Engine/Debug/Windows/DebugUIWindowTextureViewer.h"

// External includes
#include <External/imgui/imgui.h>

// Std includes
#include <filesystem>
#include <algorithm>

DebugUIWindowFileExplorer::DebugUIWindowFileExplorer()
{
	mFolderTexture = gResourceManager.GetResource<TextureResource>("Assets/Debug/Icon_Folder.png");
	mFileTexture = gResourceManager.GetResource<TextureResource>("Assets/Debug/Icon_File.png");
}

namespace fs = std::filesystem;

void DebugUIWindowFileExplorer::Update(float inDeltaTime)
{
	ImGui::Begin("File Explorer", &mOpen);

	fs::path directory_path = mCurrentPath;

	Array<String> folders = ListFoldersInDirectory(directory_path);

	for (String& folder : folders)
	{
		if (ImGui::Button((folder + "##Path").c_str()))
		{
			// Replace the current path with the new path
			size_t start_pos = mCurrentPath.find(folder);
			if (start_pos != std::string::npos)
			{
				String new_path = "";
				for (String& f : folders)
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
		if (folder != folders.back())
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

			Array<std::filesystem::directory_entry> directory_entries;
			Array<std::filesystem::directory_entry> file_entries;

			for (const auto& entry : fs::directory_iterator(directory_path))
			{
				if (entry.is_directory())
					directory_entries.push_back(entry);
				else if (entry.is_regular_file())
					file_entries.push_back(entry);
			}

			for (std::filesystem::directory_entry entry : directory_entries)
				UpdateEntry(entry, button_count, buttons_per_row);

			for (std::filesystem::directory_entry entry : file_entries)
				UpdateEntry(entry, button_count, buttons_per_row);
		}
	}
	catch (fs::filesystem_error& e)
	{
		std::cerr << e.what() << std::endl;
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
		if (gIsValidTextureExtension(file_path))
			texture = gResourceManager.GetResource<TextureResource>(file_path);
		else
			texture = mFileTexture;

		mTextures[file_path] = texture;
		if (ImGui::ImageButton(file_path.c_str(), static_cast<ImTextureID>(mTextures[file_path]->mTexture), ImVec2(mIconSize.x, mIconSize.y)))
		{
			gLog("Clicked on file: %s", file_path.c_str());
			if (!gDebugUIWindowManager.WindowExists<DebugUIWindowTextureViewer>())
				gDebugUIWindowManager.CreateWindow<DebugUIWindowTextureViewer>();

			SharedPtr<DebugUIWindowTextureViewer> texture_viewer = SPCast<DebugUIWindowTextureViewer>(
				gDebugUIWindowManager.GetWindow<DebugUIWindowTextureViewer>());

			texture_viewer->SetTexture(texture);
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

Array<String> DebugUIWindowFileExplorer::ListFoldersInDirectory(const fs::path& inDirectoryPath)
{
	Array<String> folders;
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
