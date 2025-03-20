/*
#pragma once
// Core includes
#include <DuckCore/Math/Vector.h>

// Engine includes
#include <Engine/Objects/Object.h>

class TextureResource;

class Sprite : public Object
{
public:
	Sprite(const TextureResource& inTextureResource, const DC::IVec2& inBottomLeft, const DC::IVec2& inTopRight);
	virtual ~Sprite() override;

	const TextureResource& GetTextureResource() const { return *mTextureResource; }
	const DC::IVec2& GetBottomLeft() const { return mBottomLeft; }
	const DC::IVec2& GetTopRight() const { return mTopRight; }

private:
	const DC::Ref<const TextureResource> mTextureResource;
	DC::IVec2 mBottomLeft;
	DC::IVec2 mTopRight;
};

*/