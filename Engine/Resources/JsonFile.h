#pragma once
#include <DuckCore/Containers/File/File.h>
#include <DuckCore/Utilities/Json.h>

class JsonFile : public DC::File
{
public:
	explicit JsonFile(DC::String aPath) : File(Move(aPath)) {}
	explicit JsonFile(const DC::Json& aJson) : File(aJson) {}

	virtual void Load() override;
	virtual void WriteToDisk() override;

	DC::Json& GetJson() { return mJson; }
	const DC::Json& GetJson() const { return mJson; }

private:
	DC::Json mJson;
};

void FromJson(const DC::Json& aJson, DC::Ref<JsonFile>& outJsonFile);