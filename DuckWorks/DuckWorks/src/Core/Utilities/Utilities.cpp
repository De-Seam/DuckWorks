#include "Precomp.h"
#include "Core/Utilities/Utilities.h"

bool gIsValidTextureExtension(const std::string& inFilePath)
{
	String lower_case_path;
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
	String lower_case_path;
	// Convert file path to lower case for case-insensitive comparison
	std::ranges::transform(inFilePath.begin(), inFilePath.end(), std::back_inserter(lower_case_path),
							[](unsigned char c) { return SCast<unsigned char>(std::tolower(c)); });

	const static String sExtension = ".lua";

	return lower_case_path.size() >= sExtension.size() &&
		lower_case_path.compare(lower_case_path.size() - sExtension.size(), sExtension.size(), sExtension) == 0;
}

void to_json(Json& outJson, RTTIBaseClass* inVariable)
{
	gAssert(inVariable != nullptr);
	outJson = inVariable->Serialize();
}

void to_json(Json& outJson, const SharedPtr<RTTIBaseClass>& inVariable)
{
	gAssert(inVariable != nullptr);
	outJson = inVariable->Serialize();
}
