#pragma once
#include <DuckCore/Containers/File.h>
#include <DuckCore/Utilities/Json.h>

class JsonFile : public DC::File
{
public:
	JsonFile(DC::String inPath) : DC::File(gMove(inPath)) {}
	virtual void Load() override;
	virtual void WriteToDisk() override;

	DC::Json& GetJson() { return mJson; }
	const DC::Json& GetJson() const { return mJson; }

private:
	DC::Json mJson;
};