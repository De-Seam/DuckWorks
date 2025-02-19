#include <Engine/Files/FileManager.h>

#include <fstream>

using namespace DC;

void FileManager::WriteToFile(const DC::String& inPath, const DC::String& inContents)
{
	// If this file is already open, write to it and write it to disk.
	if (WeakRef<File>* file = mFiles.Find(inPath); file != nullptr && file->IsAlive())
	{
		Ref<File> file_ref = file->Ref();
		file_ref->SetContents(inContents);
		file_ref->WriteToDisk();
		return;
	}

	// If this file is not open, we don't need to open it. Just write to the file directly.
	std::ofstream file(*inPath);
	if (!file.is_open())
	{
		// Extract the directory part of the path
        std::filesystem::path file_path(*inPath);
        std::filesystem::path dir_path = file_path.parent_path();

		// Ensure the directory exists
        if (!std::filesystem::exists(dir_path))
            std::filesystem::create_directories(dir_path);

		file.open(*inPath);
		if (!file.is_open())
		{
			gAssert(false);
			gLog(ELogLevel::Error, String::sFormatted("Unable to open file %s for writing", *inPath));
		}
	}

	file << *inContents;
	file.close();
}

bool FileManager::FileExists(const DC::String& inPath) const
{
	// Check if file exists on disk.
	std::ifstream file(*inPath);
	return file.is_open();
}
