#include "Precomp.h"
#include "Core/Utilities/Utilities.h"

// Engine includes. TODO: Remove these includes
#include "Engine/Resources/ResourceManager.h"
#include "Engine/Resources/ResourceTypes/TextureResource.h"

#include "External/box2d/box2d.h"

bool gIsValidTextureExtension(const std::string& inFilePath)
{
	String lower_case_path;
	// Convert file path to lower case for case-insensitive comparison
	std::ranges::transform(inFilePath.begin(), inFilePath.end(), std::back_inserter(lower_case_path),
							[](unsigned char c) { return SCast<unsigned char>(std::tolower(c)); });

	// List of supported image extensions
	const std::string extensions[] = {".png", ".jpg", ".jpeg", ".bmp", ".gif", ".tga", ".webp"};

	for (const std::string& ext : extensions)
	{
		if (lower_case_path.size() >= ext.size() &&
			lower_case_path.compare(lower_case_path.size() - ext.size(), ext.size(), ext) == 0)
		{
			return true;
		}
	}

	return false;
}

void gChangeB2BoxSize(b2Body* ioBody, const fm::vec2& inNewHalfSize)
{
	// Assuming the body has only one fixture that we want to replace.
	// If the body might have multiple fixtures, you may need to iterate over them.
	for (b2Fixture* f = ioBody->GetFixtureList(); f; /*no increment here*/)
	{
		// Destroy the existing fixture
		b2Fixture* fixtureToDestroy = f;
		f = f->GetNext(); // Move to the next fixture in the list
		ioBody->DestroyFixture(fixtureToDestroy);
	}

	// Define the new shape with the desired size
	b2PolygonShape boxShape;
	boxShape.SetAsBox(inNewHalfSize.x, inNewHalfSize.y);

	// Create a fixture definition using the new shape
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	// Set other fixture properties as needed
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.5f;

	// Add the new fixture to the body
	ioBody->CreateFixture(&fixtureDef);
}

void to_json(Json& outJson, const SharedPtr<TextureResource>& inVariable)
{
	outJson = inVariable->Serialize();
}

void from_json(const Json& inJson, SharedPtr<TextureResource>& outVariable)
{
	outVariable = gResourceManager.GetResource<TextureResource>(inJson);
	outVariable->Deserialize(inJson);
}
