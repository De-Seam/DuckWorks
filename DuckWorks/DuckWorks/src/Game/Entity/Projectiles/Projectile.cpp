#include "Precomp.h"
#include "Game/Entity/Projectiles/Projectile.h"

// Engine includes
#include "Engine/Entity/Components.h"
#include "Engine/Entity/Components/CollisionComponent.h"
#include "Engine/Resources/ResourceManager.h"

Json Projectile::Serialize()
{
	Json json = Base::Serialize();

	JSON_SAVE(json, mVelocity);

	return json;
}

void Projectile::Deserialize(const Json& inJson)
{
	Base::Deserialize(inJson);

	JSON_LOAD(inJson, mVelocity);
}

Projectile::Projectile()
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
