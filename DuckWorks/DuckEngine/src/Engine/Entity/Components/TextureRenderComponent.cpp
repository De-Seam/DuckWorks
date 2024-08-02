#include <Engine/Precomp.h>
#include <Engine/Entity/Components/TextureRenderComponent.h>

void TextureRenderComponent::SetTexture(TextureResource* inTexture) 
{
	mTexture = inTexture;
	mRectangle.setTexture(&mTexture->GetTexture());
}

void TextureRenderComponent::SetTransform(const Transform2D& inTransform) 
{
	Transform2D world_transform = CalculateWorldTransform(inTransform);
	mRectangle.setPosition(world_transform.mPosition);
	mRectangle.setScale(world_transform.mScale);
	mRectangle.setRotation(world_transform.mRotation);
}

void TextureRenderComponent::SetSize(const Vec2& inSize) 
{
	mRectangle.setSize(inSize);
}
