#include <Engine/Resources/JsonFile.h>

void JsonFile::Load()
{
	File::Load();

	mJson = DC::Json(GetContents());
}

void JsonFile::WriteToDisk()
{
	mContents = mJson.dump(4);
	File::WriteToDisk();
}
