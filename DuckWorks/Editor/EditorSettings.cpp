#include <Editor/EditorSettings.h>

#include <Engine/Resources/ResourceManager.h>

using namespace DC;

Json EditorSettings::ToJson() const
{
	Json json;

	json["mDefaultTextureResource"] = mDefaultTextureResource;

	return json;
}

void EditorSettings::FromJson(const Json& inJson)
{
	mDefaultTextureResource = inJson["mDefaultTextureResource"];
}
