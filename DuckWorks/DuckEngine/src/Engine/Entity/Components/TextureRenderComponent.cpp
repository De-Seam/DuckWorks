#include <Engine/Precomp.h>
#include <Engine/Entity/Components/TextureRenderComponent.h>

void TextureRenderComponent::SetTexture(TextureResource* inTexture) 
{
	mTexture = inTexture;
	mSprite.setTexture(mTexture->GetTexture());
}

void TextureRenderComponent::SetTransform(const Transform2D& inTransform) 
{
	Transform2D world_transform = CalculateWorldTransform(inTransform);
	mSprite.setPosition(world_transform.mPosition);
	mSprite.setScale(world_transform.mScale);
	mSprite.setRotation(world_transform.mRotation);
}
