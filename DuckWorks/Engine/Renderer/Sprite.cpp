#include <Engine/Renderer/Sprite.h>

Sprite::Sprite(const TextureResource& inTextureResource, const DC::IVec2& inBottomLeft, const DC::IVec2& inTopRight) :
	mTextureResource(&inTextureResource),
	mBottomLeft(inBottomLeft),
	mTopRight(inTopRight)
{ }

Sprite::~Sprite() = default;
