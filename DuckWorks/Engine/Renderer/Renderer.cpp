#include <Engine/Renderer/Renderer.h>

#include <DuckCore/Math/Transform.h>

#include <Engine/Engine.h>
#include <Engine/Events/SDLEventManager.h>
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
		gLog(LogLevel::Error, "Failed to create window\n");
		gAssert(false);
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
	if (mRenderer == nullptr)
	{
		gLog(LogLevel::Error, "Failed to create renderer\n");
		gAssert(false);
	}

	// Initialize ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls

	// Setup ImGui style
    ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(mWindow, mRenderer);
    ImGui_ImplSDLRenderer2_Init(mRenderer);

	gEngine->GetManager<SDLEventManager>();
}

Renderer::~Renderer()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
}

void Renderer::Init()
{
	Base::Init();
}

void Renderer::Shutdown()
{
	Base::Shutdown();
}

void Renderer::BeginFrame()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);

	// Start the Dear ImGui frame
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void Renderer::EndFrame()
{
	ImGui::Render();
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), mRenderer);
	SDL_RenderPresent(mRenderer);
}

void Renderer::SetWindowSize(IVec2 inSize)
{
	SDL_SetWindowSize(mWindow, inSize.mX, inSize.mY);
}

SDL_Texture* Renderer::CreateTexture(DC::IVec2 inSize)
{
	return SDL_CreateTexture(mRenderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,inSize.mX,inSize.mY);
}

void Renderer::DrawTexture(SDL_Texture* inTexture, const DC::Transform2D& inTransform)
{
	SDL_FRect dest_rect;
	dest_rect.x = inTransform.mPosition.mX;
	dest_rect.y = inTransform.mPosition.mY;
	dest_rect.w = inTransform.mHalfSize.mX * 2;
	dest_rect.h = inTransform.mHalfSize.mY * 2;

	SDL_RenderCopyExF(mRenderer, inTexture, nullptr, &dest_rect, inTransform.mRotation, nullptr, SDL_FLIP_NONE);
}

void Renderer::DrawSprite(const Sprite& inSprite, const Transform2D& inTransform)
{
	SDL_Rect src_rect;
	src_rect.x = inSprite.GetBottomLeft().mX;
	src_rect.y = inSprite.GetBottomLeft().mY;
	src_rect.w = inSprite.GetTopRight().mX - inSprite.GetBottomLeft().mX;
	src_rect.h = inSprite.GetTopRight().mY - inSprite.GetBottomLeft().mY;

	SDL_FRect dest_rect;
	dest_rect.x = inTransform.mPosition.mX;
	dest_rect.y = inTransform.mPosition.mY;
	dest_rect.w = inTransform.mHalfSize.mX * 2;
	dest_rect.h = inTransform.mHalfSize.mY * 2;

	SDL_RenderCopyExF(mRenderer, inSprite.GetTextureResource().GetTexture(), &src_rect, &dest_rect, inTransform.mRotation, nullptr, SDL_FLIP_NONE);
}

Renderer::ScopedRenderTarget::ScopedRenderTarget(SDL_Texture* inRenderTarget)
{
	Renderer& renderer = gEngine->GetManager<Renderer>();
	SDL_Renderer* sdl_renderer = renderer.GetRenderer();
	mPreviousRenderTarget = SDL_GetRenderTarget(sdl_renderer);
	SDL_SetRenderTarget(sdl_renderer,inRenderTarget);
}

Renderer::ScopedRenderTarget::~ScopedRenderTarget()
{
	Renderer& renderer = gEngine->GetManager<Renderer>();
	SDL_Renderer* sdl_renderer = renderer.GetRenderer();
	SDL_SetRenderTarget(sdl_renderer,mPreviousRenderTarget);
}
