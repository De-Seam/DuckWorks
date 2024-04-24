#include "Precomp.h"
#include "Game/Entity/Projectiles/Projectile.h"

// Engine includes
#include "Engine/Resources/ResourceManager.h"

RTTI_CLASS_DEFINITION(Projectile)

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

Projectile::Projectile() 
{ }

void Projectile::Init(const InitParams& inInitParams)
{
	Base::Init(inInitParams);

	SharedPtr<TextureResource> texture = gResourceManager.GetResource<TextureResource>("Assets/Projectiles/Projectile.png");
	AddComponent<TextureRenderComponent>();
}

void Projectile::Update(float inDeltaTime)
{
	PROFILE_SCOPE(Projectile::Update)

	MoveTo(GetPosition() + mVelocity * inDeltaTime);
}
