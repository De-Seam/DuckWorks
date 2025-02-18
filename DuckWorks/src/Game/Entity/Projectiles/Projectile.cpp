#include "Precomp.h"
#include "Game/Entity/Projectiles/Projectile.h"

// Engine includes
#include "Engine/Entity/Components.h"
#include "Engine/Entity/Components/CollisionComponent.h"
#include "Engine/Resources/ResourceManager.h"

using namespace DC;

RTTI_CLASS_DEFINITION(Projectile, StandardAllocator)

Json Projectile::Serialize()
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mVelocity);

	return json;
}

void Projectile::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	JSON_TRY_LOAD(inJson, mVelocity);
}

Projectile::Projectile(const ConstructParameters& inConstructParameters)
	: Base(inConstructParameters),
	mVelocity(inConstructParameters.mVelocity)
{
	TextureRenderComponent::ConstructParameters texture_render_component_parameters;
	texture_render_component_parameters.mTexture = gResourceManager.GetResource<TextureResource>("Assets/Projectiles/Projectile.png");
	AddComponent<TextureRenderComponent>(texture_render_component_parameters);

	CollisionComponent::ConstructParameters collision_component_params;
	collision_component_params.mType = CollisionObject::EType::Dynamic;
	AddComponent<CollisionComponent>(collision_component_params);
}

void Projectile::Update(float inDeltaTime)
{
	PROFILE_SCOPE(Projectile::Update)

	SetPosition(GetPosition() + mVelocity * inDeltaTime);
}
