#pragma once
// Engine includes
#include <Engine/Entity/Components/WorldComponent.h>
#include <Engine/Resources/TextureResource.h>

// External includes
#include <External/SFML/Graphics/CircleShape.hpp>
#include <External/SFML/Graphics/ConvexShape.hpp>
#include <External/SFML/Graphics/RectangleShape.hpp>

enum class EShapeType : uint8
{
	Rectangle,
	Circle, 
	Convex,
	None
};

class TextureRenderComponent : public WorldComponent
{
	RTTI_CLASS(TextureRenderComponent, WorldComponent)

public:
	TextureRenderComponent();

	void SetTexture(TextureResource* inTexture);
	TextureResource* GetTexture() { return mTexture; }
	const TextureResource* GetTexture() const { return mTexture; }

	void SetShape(const sf::RectangleShape& inRectangleShape);
	void SetShape(const sf::CircleShape& inCircleShape);
	void SetShape(const sf::ConvexShape& inRectangleShape);

	EShapeType GetShapeType() const { return mShapeType; }
	sf::Shape* GetShape() { return &mRectangleShape; }
	const sf::Shape* GetShape() const { return &mRectangleShape; }
	sf::RectangleShape& GetRectangleShape() { gAssert(mShapeType == EShapeType::Rectangle); return mRectangleShape; }
	sf::CircleShape& GetCircleShape() { gAssert(mShapeType == EShapeType::Circle); return mCircleShape; }
	sf::ConvexShape& GetConvexShape() { gAssert(mShapeType == EShapeType::Convex); return mConvexShape; }

private:

	Ref<TextureResource> mTexture = nullptr;
	EShapeType mShapeType = EShapeType::None;
	union
	{
		sf::RectangleShape mRectangleShape;
		sf::CircleShape mCircleShape;
		sf::ConvexShape mConvexShape;
	};
};

class RectangleRenderComponent : public TextureRenderComponent
{
	RTTI_CLASS(TextureRenderComponent, TextureRenderComponent)

public:
	sf::RectangleShape mRectangleShape;
};