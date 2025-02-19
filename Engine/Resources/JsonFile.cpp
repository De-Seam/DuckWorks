#include <Engine/Resources/JsonFile.h>

void JsonFile::Load()
{
	File::Load();

	if (GetContents().IsEmpty())
		return;

	mJson = DC::Json::parse(GetContents().CStr());
}

void JsonFile::WriteToDisk()
{
	mContents = mJson.dump(4);
	File::WriteToDisk();
}
