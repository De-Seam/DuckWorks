#include "Precomp.h"
#include "Game/Entity/SolidObstacle.h"

// Engine includes
#include "Engine/World/World.h"
#include "Engine/Resources/ResourceManager.h"

// External includes
#include <External/box2d/box2d.h>


RTTI_EMPTY_SERIALIZE_DEFINITION(SolidObstacle)

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
	fixture_def.restitutionThreshold = 100000.f;

	TextureRenderComponent& texture_render_component = AddComponent<TextureRenderComponent>();
	texture_render_component.mTexture = gResourceManager.GetResource<TextureResource>("Assets/DefaultTexture.png");

	CollisionComponent& collision_component = AddComponent<CollisionComponent>();

	CollisionObject::InitParams params;
	params.mTransform = GetTransform();
	params.mType = CollisionObject::EType::Static;
	collision_component.mCollisionObjectHandle = GetWorld()->GetCollisionWorld()->CreateCollisionObject(params);
}
