#pragma once
#include <DuckCore/Containers/HashMap.h>
#include <DuckCore/Containers/File/File.h>
#include <DuckCore/Managers/Manager.h>

class FileManager : public DC::Manager
{
	MANAGER_BASE_CLASS(FileManager)
public:
	template<typename taType>
	void RegisterFileExtension(DC::String inExtension); // Register a file extension to be used for taType

	template<typename taType>
	DC::Ref<taType> Get(const DC::String& inPath);

	void WriteToFile(const DC::String& inPath, const DC::String& inContents);

	bool FileExists(const DC::String& inPath) const;

private:
	DC::HashMap<DC::String, DC::WeakRef<DC::File>> mFiles;
	DC::HashMap<uint64, DC::String> mFileExtensions; // Maps file class to file extension
};

template <typename taType>
void FileManager::RegisterFileExtension(DC::String inExtension)
{
	gAssert(!inExtension.Contains('.'), "Extensions should be registered without the dot");

	uint64 type_id = typeid(taType).hash_code();
	gAssert(!mFileExtensions.Contains(type_id) || mFileExtensions.At(type_id) == inExtension);
	mFileExtensions[type_id] = Move(inExtension);
}

template <typename taType>
DC::Ref<taType> FileManager::Get(const DC::String& inPath)
{
	DC::HashMap<DC::String, DC::WeakRef<DC::File>>::Iterator iter = mFiles.Find(inPath);
	DC::WeakRef<DC::File>* file = iter.IsValid() ? &iter.GetValue() : nullptr;

#ifdef _ASSERTS
	// If there is no dot in the path there is no extension
	if (const int dot_pos = inPath.FindLastCharOccurence('.'); dot_pos != -1)
	{
		DC::String file_extension = inPath.SubStr(dot_pos + 1, inPath.Length() - 1);
		uint64 type_id = typeid(taType).hash_code();

		DC::HashMap<uint64, DC::String>::Iterator registered_file_extension = mFileExtensions.Find(type_id);
		DC::String* registered_file_extension_ptr = registered_file_extension.IsValid() ? &registered_file_extension.GetValue() : nullptr;
		if (registered_file_extension_ptr == nullptr)
		{

			Log(DC::ELogLevel::Warning, DC::String::sFormatted("Loading file %s, but this extension is not registered.", *inPath));
		}
		else
		{
			gAssert((*registered_file_extension_ptr) == file_extension, "File extension is registered for a different file type.");
		}
	}
#endif

	// If we haven't loaded the file, or it has been unloaded already, we (re)load it.
	if (file == nullptr || !file->IsAlive())
	{
		DC::Ref<DC::File> file_ref = new taType(inPath);
		mFiles[inPath] = file_ref;
		file_ref->Load();
		return file_ref;
	}

	return file->Ref();
}
