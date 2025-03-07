#include <Engine/Renderer/Renderer.h>

#include <DuckCore/Core/Log.h>
#include <DuckCore/Managers/Managers.h>
#include <DuckCore/Math/Transform.h>

#include <Engine/Engine.h>
#include <Engine/Events/SDLEventManager.h>
#include <Engine/Renderer/Events.h>
#include <Engine/Renderer/Sprite.h>
#include <Engine/Renderer/TextureResource.h>

#include <External/imgui/imgui.h>
#include <External/imgui/imgui_impl_sdl2.h>
#include <External/imgui/imgui_impl_sdlrenderer2.h>

#include <SDL/SDL.h>

using namespace DC;

Renderer::Renderer()
{
	mWindow = SDL_CreateWindow("DuckWorks", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);

	if (mWindow == nullptr)
	{
		Log(ELogLevel::Error, "Failed to create window\n");
		gAssert(false);
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
	if (mRenderer == nullptr)
	{
		Log(ELogLevel::Error, "Failed to create renderer\n");
		gAssert(false);
	}

	// Initialize ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::LoadIniSettingsFromDisk("imgui.ini");

	// Setup ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(mWindow, mRenderer);
	ImGui_ImplSDLRenderer2_Init(mRenderer);
}

Renderer::~Renderer()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
}

void Renderer::BeginFrame()
{
	gAssert(IsMainThread());

	PreRendererBeginFrameEvent pre_event;
	Get<EventManager>().SendEvent(pre_event);

	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);

	// Start the Dear ImGui frame. We do this here for now, but if we ever create an ImGui manager we may want to move it there with an event listener.
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	PostRendererBeginFrameEvent post_event;
	Get<EventManager>().SendEvent(post_event);
}

void Renderer::EndFrame()
{
	PreRendererEndFrameEvent pre_event;
	Get<EventManager>().SendEvent(pre_event);

	ImGui::Render();
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), mRenderer);
	SDL_RenderPresent(mRenderer);

	PostRendererEndFrameEvent post_event;
	Get<EventManager>().SendEvent(post_event);
}

void Renderer::SetWindowSize(IVec2 aSize)
{
	SDL_SetWindowSize(mWindow, aSize.mX, aSize.mY);
}

SDL_Texture* Renderer::CreateTexture(IVec2 aSize)
{
	return SDL_CreateTexture(mRenderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,aSize.mX,aSize.mY);
}

void Renderer::DestroyTexture(SDL_Texture*& aTexture)
{
	gAssert(aTexture != nullptr);
	SDL_DestroyTexture(aTexture);
	aTexture = nullptr;
}

Ref<RenderTarget> Renderer::CreateRenderTarget(IVec2 aSize)
{
	SDL_Texture* texture = SDL_CreateTexture(mRenderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,aSize.mX,aSize.mY);
	return new RenderTarget(texture, aSize);
}

void Renderer::sDestroyTexture(SDL_Texture*& aTexture)
{
	SDL_DestroyTexture(aTexture);
	aTexture = nullptr;
}

void Renderer::DrawTexture(SDL_Texture* aTexture, const DC::Transform2D& aTransform)
{
	SDL_FRect dest_rect;
	dest_rect.x = aTransform.mPosition.mX;
	dest_rect.y = aTransform.mPosition.mY;
	dest_rect.w = aTransform.mHalfSize.mX * 2;
	dest_rect.h = aTransform.mHalfSize.mY * 2;

	SDL_RenderCopyExF(mRenderer, aTexture, nullptr, &dest_rect, aTransform.mRotation, nullptr, SDL_FLIP_NONE);
}

/*
void Renderer::DrawSprite(const Sprite& aSprite, const Transform2D& aTransform)
{
	SDL_Rect src_rect;
	src_rect.x = aSprite.GetBottomLeft().mX;
	src_rect.y = aSprite.GetBottomLeft().mY;
	src_rect.w = aSprite.GetTopRight().mX - aSprite.GetBottomLeft().mX;
	src_rect.h = aSprite.GetTopRight().mY - aSprite.GetBottomLeft().mY;

	SDL_FRect dest_rect;
	dest_rect.x = aTransform.mPosition.mX;
	dest_rect.y = aTransform.mPosition.mY;
	dest_rect.w = aTransform.mHalfSize.mX * 2;
	dest_rect.h = aTransform.mHalfSize.mY * 2;

	SDL_RenderCopyExF(mRenderer, aSprite.GetTextureResource().GetTexture(), &src_rect, &dest_rect, aTransform.mRotation, nullptr, SDL_FLIP_NONE);
}
*/

void Renderer::DrawRectangle(const FRect2D& aRect, const RGBA& aColor) 
{
	SDL_FRect rect;
	rect.x = aRect.mX;
	rect.y = aRect.mY;
	rect.w = aRect.mWidth;
	rect.h = aRect.mHeight;

	SDL_SetRenderDrawColor(mRenderer, aColor.mR, aColor.mG, aColor.mB, aColor.mA);
	SDL_RenderFillRectF(mRenderer, &rect);
}

void Renderer::DrawRectangle(const IRect2D& aRect, const RGBA& aColor) 
{
	SDL_Rect rect;
	rect.x = aRect.mX;
	rect.y = aRect.mY;
	rect.w = aRect.mWidth;
	rect.h = aRect.mHeight;

	SDL_SetRenderDrawColor(mRenderer, aColor.mR, aColor.mG, aColor.mB, aColor.mA);
	SDL_RenderFillRect(mRenderer, &rect);
}

void Renderer::Clear(RGBA aColor)
{
	SDL_SetRenderDrawColor(mRenderer, aColor.mR, aColor.mG, aColor.mB, aColor.mA);
	SDL_RenderClear(mRenderer);
}

Renderer::ScopedRenderTarget::ScopedRenderTarget(RenderTarget& aRenderTarget) :
	mRenderTarget(&aRenderTarget)
{
	Renderer& renderer = Managers::sGet<Renderer>();
	mPreviousRenderTarget = renderer.GetRenderTarget();

	renderer.SetRenderTarget(mRenderTarget);
}

Renderer::ScopedRenderTarget::~ScopedRenderTarget()
{
	Renderer& renderer = Managers::sGet<Renderer>();

	gAssert(mPreviousRenderTarget == renderer.mPreviousRenderTarget, "ScopedRenderTarget is stack-based. If this assert triggers, a previous render target has left its scope while the current render target is still active.");
	renderer.SetRenderTarget(mPreviousRenderTarget);
}

void Renderer::SetRenderTarget(RenderTarget* aRenderTarget)
{
	mPreviousRenderTarget = mRenderTarget;
	mRenderTarget = aRenderTarget;
	SDL_SetRenderTarget(mRenderer, mRenderTarget == nullptr ? nullptr : mRenderTarget->GetTexture());
}
