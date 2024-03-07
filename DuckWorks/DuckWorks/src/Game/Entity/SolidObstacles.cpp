#include "Precomp.h"
#include "Game/Entity/SolidObstacle.h"

// External includes
#include <External/box2d/box2d.h>

#include "Engine/Resources/ResourceManager.h"

SolidObstacle::SolidObstacle(World* inWorld)
	: Actor(inWorld)
{
	b2BodyDef body_def;
	body_def.type = b2_staticBody;
	body_def.position = b2Vec2(GetPosition().x, GetPosition().y);
	body_def.angle = GetRotation();

	b2PolygonShape dynamic_box;
	dynamic_box.SetAsBox(GetHalfSize().x, GetHalfSize().y);

	b2FixtureDef fixture_def;
	fixture_def.shape = &dynamic_box;
	fixture_def.density = 1.0f;
	fixture_def.friction = 1.f;

	CreatePhysicsBody(body_def, fixture_def);

	TextureRenderComponent& texture_render_component = AddComponent<TextureRenderComponent>();
	texture_render_component.mTexture = gResourceManager.GetResource<TextureResource>("Assets/DefaultTexture.png");

	AddComponent<PhysicsPositionOrRotationUpdatedTag>();
	AddComponent<PhysicsSizeUpdatedTag>();
}
