#include "Precomp.h"
#include "Core/Utilities/Utilities.h"

// Engine includes. TODO: Remove these includes
#include "Engine/Resources/ResourceManager.h"
#include "Engine/Resources/ResourceTypes/TextureResource.h"
#include "Engine/World/World.h"

// Game includes. TODO: Remove these includes
#include "Game/App/App.h"

bool gIsValidTextureExtension(const std::string& inFilePath)
{
	std::string lower_case_path;
	// Convert file path to lower case for case-insensitive comparison
	std::ranges::transform(inFilePath.begin(), inFilePath.end(), std::back_inserter(lower_case_path),
							[](unsigned char c) { return SCast<unsigned char>(std::tolower(c)); });

	// List of supported image extensions
	const static std::string sExtensions[] = {".png", ".jpg", ".jpeg", ".bmp", ".gif", ".tga", ".webp"};

	for (const std::string& ext : sExtensions)
	{
		if (lower_case_path.size() >= ext.size() &&
			lower_case_path.compare(lower_case_path.size() - ext.size(), ext.size(), ext) == 0)
		{
			return true;
		}
	}

	return false;
}

bool gIsValidLuaExtension(const std::string& inFilePath)
{
	std::string lower_case_path;
	// Convert file path to lower case for case-insensitive comparison
	std::ranges::transform(inFilePath.begin(), inFilePath.end(), std::back_inserter(lower_case_path),
							[](unsigned char c) { return SCast<unsigned char>(std::tolower(c)); });

	const static std::string sExtension = ".lua";

	return lower_case_path.size() >= sExtension.size() &&
		lower_case_path.compare(lower_case_path.size() - sExtension.size(), sExtension.size(), sExtension) == 0;
}

// TextureResource
void to_json(Json& outJson, const SharedPtr<TextureResource>& inVariable)
{
	outJson = inVariable->Serialize();
}

void from_json(const Json& inJson, SharedPtr<TextureResource>& outVariable)
{
	outVariable = gResourceManager.GetResource<TextureResource>(inJson["mFile"]);
	outVariable->Deserialize(inJson);
}

// AABB
void to_json(Json& outJson, const AABB& inVariable)
{
	outJson = Json{inVariable.mMin, inVariable.mMax};
}

void from_json(const Json& inJson, AABB& outVariable)
{
	inJson.at(0).get_to(outVariable.mMin);
	inJson.at(1).get_to(outVariable.mMax);
}
