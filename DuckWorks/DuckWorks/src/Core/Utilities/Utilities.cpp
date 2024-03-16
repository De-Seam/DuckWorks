#include "Precomp.h"
#include "Core/Utilities/Utilities.h"

// Engine includes. TODO: Remove these includes
#include "Engine/Resources/ResourceManager.h"
#include "Engine/Resources/ResourceTypes/TextureResource.h"
#include "Engine/World/World.h"

// Game includes. TODO: Remove these includes
#include "Game/App/App.h"

// External includes
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

struct BoxProperties
{
	float width, height, rotation;
};


BoxProperties ComputeBoxProperties(const std::vector<b2Vec2>& vertices)
{
	float minX = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::lowest();
	float minY = std::numeric_limits<float>::max();
	float maxY = std::numeric_limits<float>::lowest();

	for (const auto& v : vertices)
	{
		minX = std::min(minX, v.x);
		maxX = std::max(maxX, v.x);
		minY = std::min(minY, v.y);
		maxY = std::max(maxY, v.y);
	}

	// Assuming the first and second vertices form one of the box's edges
	float dx = vertices[1].x - vertices[0].x;
	float dy = vertices[1].y - vertices[0].y;
	float rotation = std::atan2(dy, dx); // Rotation in radians

	BoxProperties props;
	props.width = maxX - minX;
	props.height = maxY - minY;
	props.rotation = rotation * (180.f / SCast<float>(M_PI)); // Convert to degrees for convenience

	return props;
}

// b2Vec2
void to_json(Json& outJson, const b2Vec2& inVariable)
{
	outJson = {inVariable.x, inVariable.y};
}

void from_json(const Json& inJson, b2Vec2& outVariable)
{
	inJson.at(0).get_to(outVariable.x);
	inJson.at(1).get_to(outVariable.y);
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
