#pragma once
#include <DuckCore/Math/Vector.h>
#include <DuckCore/RTTI/Ref.h>

struct SDL_Texture;

class RenderTarget : public DC::RefClass
{
public:
	RenderTarget(SDL_Texture* aTexture, const DC::IVec2& aSize);
	virtual ~RenderTarget() override;

	SDL_Texture* GetTexture() { return mTexture; }
	const SDL_Texture* GetTexture() const { return mTexture; }
	const DC::IVec2& GetSize() const { return mSize; }

private:
	SDL_Texture* mTexture = nullptr;
	DC::IVec2 mSize;
};

void ToJson(DC::Json& aJson, const RenderTarget* aRenderTarget);
void FromJson(const DC::Json& aJson, DC::Ref<RenderTarget>& aRenderTarget);
