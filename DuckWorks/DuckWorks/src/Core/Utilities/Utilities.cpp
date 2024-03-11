#include "Precomp.h"
#include "Core/Utilities/Utilities.h"

// Engine includes. TODO: Remove these includes
#include "Engine/Resources/ResourceManager.h"
#include "Engine/Resources/ResourceTypes/TextureResource.h"
#include "Engine/World/World.h"

#include "Game/App/App.h"

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

// b2Body
void to_json(Json& outJson, const b2Body* inVariable)
{
	int32 index = 0;
	Json fixtures_json;
	for (const b2Fixture* fixture = inVariable->GetFixtureList(); fixture; /*no increment here*/)
	{
		Json fixture_json;

		fixture_json["IsSensor"] = fixture->IsSensor();
		fixture_json["Density"] = fixture->GetDensity();
		fixture_json["Friction"] = fixture->GetFriction();
		fixture_json["Restitution"] = fixture->GetRestitution();
		fixture_json["RestitutionThreshold"] = fixture->GetRestitutionThreshold();
		switch (fixture->GetType())
		{
		case b2Shape::Type::e_circle:
		{
			const b2CircleShape* circle = SCast<const b2CircleShape*>(fixture->GetShape());
			fixture_json["CircleShape"]["Radius"] = circle->m_radius;
		}
		break;
		case b2Shape::Type::e_edge:
		{
			const b2EdgeShape* edge = SCast<const b2EdgeShape*>(fixture->GetShape());
			fixture_json["EdgeShape"]["Vertex1"]["x"] = edge->m_vertex1.x;
			fixture_json["EdgeShape"]["Vertex1"]["y"] = edge->m_vertex1.y;
			fixture_json["EdgeShape"]["Vertex2"]["x"] = edge->m_vertex2.x;
			fixture_json["EdgeShape"]["Vertex2"]["y"] = edge->m_vertex2.y;
		}
		break;
		case b2Shape::Type::e_polygon:
		{
			const b2PolygonShape* polygon = SCast<const b2PolygonShape*>(fixture->GetShape());
			fixture_json["PolygonShape"]["Centroid"] = polygon->m_centroid;
			fixture_json["PolygonShape"]["Count"] = polygon->m_count;
			BoxProperties properties = ComputeBoxProperties({polygon->m_vertices, polygon->m_vertices + polygon->m_count});
			fixture_json["PolygonShape"]["HalfWidth"] = properties.width * 0.5f;
			fixture_json["PolygonShape"]["HalfHeight"] = properties.height * 0.5f;
			fixture_json["PolygonShape"]["Rotation"] = properties.rotation;
			//for (int32 i = 0; i < polygon->m_count; i++)
			//{
			//	fixture_json["PolygonShape"]["Vertices"].emplace_back(polygon->m_vertices[i]);
			//	fixture_json["PolygonShape"]["Normals"].emplace_back(polygon->m_normals[i]);
			//}
		}
		break;
		case b2Shape::Type::e_chain:
		{
			const b2ChainShape* shape = SCast<const b2ChainShape*>(fixture->GetShape());
			fixture_json["ChainShape"]["Count"] = shape->m_count;
			for (int32 i = 0; i < shape->m_count; i++)
			{
				fixture_json["ChainShape"]["Vertices"].emplace_back(shape->m_vertices[i]);
			}
			fixture_json["ChainShape"]["PrevVertex"] = shape->m_prevVertex;
			fixture_json["ChainShape"]["NextVertex"] = shape->m_nextVertex;
		}
		break;
		}
		fixtures_json[std::to_string(index)] = fixture_json;
		fixture = fixture->GetNext(); // Move to the next fixture in the list
		index++;
	}
	outJson["Fixtures"] = fixtures_json;
	Json body_json;
	body_json["Position"] = inVariable->GetPosition();
	body_json["Angle"] = inVariable->GetAngle();
	body_json["LinearVelocity"] = inVariable->GetLinearVelocity();
	body_json["AngularVelocity"] = inVariable->GetAngularVelocity();
	body_json["LinearDamping"] = inVariable->GetLinearDamping();
	body_json["AngularDamping"] = inVariable->GetAngularDamping();
	body_json["AllowSleep"] = inVariable->IsSleepingAllowed();
	body_json["Awake"] = inVariable->IsAwake();
	body_json["FixedRotation"] = inVariable->IsFixedRotation();
	body_json["Bullet"] = inVariable->IsBullet();
	body_json["Type"] = inVariable->GetType();
	body_json["Enabled"] = inVariable->IsEnabled();
	body_json["GravityScale"] = inVariable->GetGravityScale();

	outJson["BodyDef"] = body_json;
}

void FromJson(const Json& inJson, b2Body*& outVariable)
{
	if (!inJson.contains("BodyDef"))
		return gLog("Error: No body definition found in the JSON object");

	Array<b2FixtureDef> fixture_defs;
	if (inJson.contains("Fixtures"))
	{
		for (const auto& [key, value] : inJson["Fixtures"].items())
		{
			// Key is the index of the fixture
			b2FixtureDef fixture_def;
			fixture_def.isSensor = value["IsSensor"];
			fixture_def.density = value["Density"];
			fixture_def.friction = value["Friction"];
			fixture_def.restitution = value["Restitution"];
			fixture_def.restitutionThreshold = value["RestitutionThreshold"];
			if (value.contains("CircleShape"))
			{
				b2CircleShape shape;
				shape.m_radius = value["CircleShape"]["Radius"];
				fixture_def.shape = new b2CircleShape(shape);
			}
			else if (value.contains("EdgeShape"))
			{
				b2EdgeShape shape;
				shape.m_vertex1.x = value["EdgeShape"]["Vertex1"]["x"];
				shape.m_vertex1.y = value["EdgeShape"]["Vertex1"]["y"];
				shape.m_vertex2.x = value["EdgeShape"]["Vertex2"]["x"];
				shape.m_vertex2.y = value["EdgeShape"]["Vertex2"]["y"];
				fixture_def.shape = new b2EdgeShape(shape);
			}
			else if (value.contains("PolygonShape"))
			{
				b2PolygonShape shape;
				shape.m_centroid = value["PolygonShape"]["Centroid"];
				//shape.m_count = value["PolygonShape"]["Count"];
				//for (int32 i = 0; i < shape.m_count; i++)
				//{
				//	shape.m_vertices[i] = value["PolygonShape"]["Vertices"][i];
				//	shape.m_normals[i] = value["PolygonShape"]["Normals"][i];
				//}
				//BoxProperties properties = ComputeBoxProperties({shape.m_vertices, shape.m_vertices + shape.m_count});
				//shape.SetAsBox(properties.width, properties.height, {0.f, 0.f}, properties.rotation);

				float half_width = value["PolygonShape"]["HalfWidth"];
				float half_height = value["PolygonShape"]["HalfHeight"];
				float rotation = value["PolygonShape"]["Rotation"];

				shape.SetAsBox(half_width, half_height, {0, 0}, rotation);
				fixture_def.shape = new b2PolygonShape(shape);
			}
			else if (value.contains("ChainShape"))
			{
				b2ChainShape shape;
				for (const auto& vertex : value["ChainShape"]["Vertices"])
				{
					shape.m_vertices[shape.m_count] = vertex;
					shape.m_count++;
				}
				shape.m_prevVertex = value["ChainShape"]["PrevVertex"];
				shape.m_nextVertex = value["ChainShape"]["NextVertex"];
				fixture_def.shape = new b2ChainShape(shape);
			}

			fixture_defs.emplace_back(fixture_def);
		}
	}

	const Json& body_json = inJson["BodyDef"];
	b2BodyDef body_def;
	body_def.position = body_json["Position"];
	body_def.angle = body_json["Angle"];
	body_def.linearVelocity = body_json["LinearVelocity"];
	body_def.angularVelocity = body_json["AngularVelocity"];
	body_def.linearDamping = body_json["LinearDamping"];
	body_def.angularDamping = body_json["AngularDamping"];
	body_def.allowSleep = body_json["AllowSleep"];
	body_def.awake = body_json["Awake"];
	body_def.fixedRotation = body_json["FixedRotation"];
	body_def.bullet = body_json["Bullet"];
	body_def.type = body_json["Type"];
	body_def.enabled = body_json["Enabled"];
	body_def.gravityScale = body_json["GravityScale"];

	// Destroy the old body if it exists
	if (outVariable != nullptr)
	{
		gApp.GetWorld()->DestroyPhysicsBody(outVariable);
	}

	outVariable = gApp.GetWorld()->CreatePhysicsBody(body_def, fixture_defs);
	//for (const b2FixtureDef& fixture_def : fixture_defs)
	//{
	//	delete fixture_def.shape;
	//}
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
