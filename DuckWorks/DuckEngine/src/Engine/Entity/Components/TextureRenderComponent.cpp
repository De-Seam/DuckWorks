#include <Engine/Precomp.h>
#include <Engine/Entity/Components/TextureRenderComponent.h>

TextureRenderComponent::TextureRenderComponent() 
{

}

void TextureRenderComponent::SetTexture(TextureResource* inTexture) 
{
	mTexture = inTexture;
	
}

void TextureRenderComponent::SetShape(const sf::RectangleShape& inRectangleShape) 
{
	mShapeType = EShapeType::Rectangle;
	mRectangleShape = inRectangleShape;
}

void TextureRenderComponent::SetShape(const sf::CircleShape& inCircleShape) 
{
	mShapeType = EShapeType::Circle;
	mCircleShape = inCircleShape;
}

void TextureRenderComponent::SetShape(const sf::ConvexShape& inRectangleShape) 
{
	mShapeType = EShapeType::Convex;
	mConvexShape = inRectangleShape;
}
