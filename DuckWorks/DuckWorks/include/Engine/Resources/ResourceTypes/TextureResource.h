#pragma once
#include "BaseResource.h"

struct SDL_Texture;

struct TextureResource : public BaseResource
{
	RTTI_CLASS(TextureResource, BaseResource)

public:
	struct ConstructParameters : public Base::ConstructParameters {};

	TextureResource(const ConstructParameters& inConstructParameters = {}) : Base(inConstructParameters) {}

	virtual ~TextureResource() override;
	virtual void Reload() override;

	virtual uint64 GetMemorySize() const override;

	SDL_Texture* mTexture = nullptr;

	IVec2 GetSize() const { return mSize; }
	int32 GetWidth() const { return mSize.mX; }
	int32 GetHeight() const { return mSize.mY; }

protected:
	virtual void LoadFromFile(const String& inFile) override;

private:
	IVec2 mSize = {};

	void DestroyTexture();
};

void from_json(const Json& inJson, SharedPtr<TextureResource>& outVariable);
