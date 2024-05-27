#include "Precomp.h"
#include "Game/Entity/Projectiles/Projectile.h"

// Engine includes
#include "Engine/Entity/Components.h"
#include "Engine/Resources/ResourceManager.h"

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
}

void Projectile::Init(const InitParams& inInitParams)
{
	Base::Init(inInitParams);
}

void Projectile::Update(float inDeltaTime)
{
	PROFILE_SCOPE(Projectile::Update)

	MoveTo(GetPosition() + mVelocity * inDeltaTime);
}
