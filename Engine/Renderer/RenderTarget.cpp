#include <Engine/Renderer/RenderTarget.h>

#include <DuckCore/Managers/Managers.h>

#include <Engine/Renderer/Renderer.h>

using namespace DC;

RenderTarget::RenderTarget(SDL_Texture* aTexture, const IVec2& aSize) :
	mTexture(aTexture),
	mSize(aSize)
{}

RenderTarget::~RenderTarget()
{
	Renderer::sDestroyTexture(mTexture);
}
