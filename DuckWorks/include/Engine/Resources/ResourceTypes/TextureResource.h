#pragma once
#include "BaseResource.h"

struct SDL_Texture;

struct TextureResource : public BaseResource
{
	RTTI_CLASS(TextureResource, BaseResource, ClassAllocator)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	using Base::Base;

	TextureResource(const ConstructParameters& inConstructParameters = {}) : Base(inConstructParameters) {}

	virtual ~TextureResource() override;
	virtual void Reload() override;

	SDL_Texture* mTexture = nullptr;

	IVec2 GetSize() const { return mSize; }
	int32 GetWidth() const { return mSize.mX; }
	int32 GetHeight() const { return mSize.mY; }

protected:
	virtual void LoadFromFile(const DC::String& inFile) override;

private:
	IVec2 mSize = {};

	void DestroyTexture();
};
