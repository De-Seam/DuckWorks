#include <Engine/Resources/JsonFile.h>

#include <DuckCore/Managers/Managers.h>

#include <Engine/Files/FileManager.h>

using namespace DC;

void JsonFile::Load()
{
	File::Load();

	if (GetContents().IsEmpty())
		return;

	mJson = Json::parse(GetContents().CStr());
}

void JsonFile::WriteToDisk()
{
	mContents = mJson.dump(4);
	File::WriteToDisk();
}

void FromJson(const Json& aJson, Ref<JsonFile>& outJsonFile)
{
	outJsonFile = Managers::sGet<FileManager>().Get<JsonFile>(aJson.get<String>());
}
