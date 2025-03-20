#include <Renderer/RenderTarget.h>

#include <DuckCore/Managers/Managers.h>

#include <Renderer/Renderer.h>

using namespace DC;

RenderTarget::RenderTarget(SDL_Texture* aTexture, const IVec2& aSize) :
	mTexture(aTexture),
	mSize(aSize)
{}

RenderTarget::~RenderTarget()
{
	Renderer::sDestroyTexture(mTexture);
}

void ToJson(Json& aJson, const RenderTarget* aRenderTarget)
{
	aJson["mSize"] = aRenderTarget->GetSize();
}

void FromJson(const Json& aJson, Ref<RenderTarget>& aRenderTarget)
{
	IVec2 size = aJson["mSize"];
	aRenderTarget = Get<Renderer>().CreateRenderTarget(size);
}
