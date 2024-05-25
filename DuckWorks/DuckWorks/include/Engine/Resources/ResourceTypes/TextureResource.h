#pragma once
#include "BaseResource.h"

struct SDL_Texture;

struct TextureResource : public BaseResource
{
	RTTI_CLASS(TextureResource, BaseResource)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	using Base::Base;

	TextureResource(const ConstructParameters& inConstructParameters = {}) : Base(inConstructParameters) {}

	virtual ~TextureResource() override;
	virtual void LoadFromFile(const String& inFile) override;
	void ReloadTexture();

	SDL_Texture* mTexture = nullptr;

	fm::ivec2 GetSize() const { return mSize; }
	int32 GetWidth() const { return mSize.x; }
	int32 GetHeight() const { return mSize.y; }

private:
	fm::ivec2 mSize = {};

	void DestroyTexture();
};
