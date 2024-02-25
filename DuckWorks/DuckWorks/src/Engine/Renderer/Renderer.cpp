#include "Precomp.h"
#include "Engine/Renderer/Renderer.h"

#include "External/imgui/imgui.h"
#include "External/imgui/imgui_impl_sdl2.h"
#include "External/imgui/imgui_impl_sdlrenderer2.h"
#include "External/SDL/SDL.h"

Renderer gRenderer;

Renderer::Renderer()
{}

Renderer::~Renderer()
{}

void Renderer::Init(const InitParams& inInitParams)
{
	gLog(LogType::Info, "Initializing Renderer");

	SDL_SetMainReady();
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		gLog(LogType::Error, "Error initializing SDL: %s\n", SDL_GetError());
		exit(1);
	}

	if (std::atexit(SDL_Quit) != 0)
	{
		gLog(LogType::Error, "Error assigning SDL_Quit to atexit.");
	}

	mWindow = SDL_CreateWindow(inInitParams.mWindowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, inInitParams.mWindowSize.x,
								inInitParams.mWindowSize.y, inInitParams.mWindowFlags);

	mRenderer = SDL_CreateRenderer(mWindow, -1, inInitParams.mRendererFlags);

	mWindowSize = inInitParams.mWindowSize;

	mCamera = std::make_unique<Camera>();

	mRenderTargetTexture = SDL_CreateTexture(mRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, inInitParams.mWindowSize.x,
											inInitParams.mWindowSize.y);
	if (!mRenderTargetTexture)
	{
		gLog(LogType::Error, "Error creating render target texture: %s\n", SDL_GetError());
	}
}

void Renderer::Shutdown()
{
	gLog(LogType::Info, "Shutting down Renderer");

	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
}

void Renderer::BeginFrame()
{
	SDL_SetRenderTarget(mRenderer, mRenderTargetTexture);

	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);

	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void Renderer::EndFrame()
{
	SDL_SetRenderTarget(mRenderer, nullptr);

	SDL_Rect src_rect = {0, 0, mWindowSize.x, mWindowSize.y};
	SDL_Rect dst_rect = {0, 0, mWindowSize.x, mWindowSize.y};
	SDL_RenderCopyEx(mRenderer, mRenderTargetTexture, &src_rect, &dst_rect, 0.0, nullptr, SDL_FLIP_VERTICAL);

	ImGui::Render();
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

	SDL_RenderPresent(mRenderer);
}

void Renderer::Update(float inDeltaTime)
{
	mCamera->Update(inDeltaTime);
}

void Renderer::DrawTexture(SDL_Texture* inTexture, const fm::vec2& inPosition, const fm::vec2& inHalfSize, float inRotation)
{
	const SDL_FRect dstRect = GetSDLFRect(inPosition, inHalfSize);
	SDL_RenderCopyExF(mRenderer, inTexture, nullptr, &dstRect, inRotation, nullptr, SDL_FLIP_NONE);
}

void Renderer::DrawTextureTinted(SDL_Texture* inTexture, const fm::vec2& inPosition, const fm::vec2& inHalfSize, float inRotation, const fm::vec4& inColor)
{
	// Calculate color components
	const uint32 argb = inColor.get_argb();
	const uint8 a = (argb >> 24) & 0xFF;
	const uint8 r = (argb >> 16) & 0xFF;
	const uint8 g = (argb >> 8) & 0xFF;
	const uint8 b = (argb >> 0) & 0xFF;

	// Set SDL render color
	SDL_SetTextureColorMod(inTexture, r, g, b);
	SDL_SetTextureAlphaMod(inTexture, a);

	const SDL_FRect dstRect = GetSDLFRect(inPosition, inHalfSize);
	SDL_RenderCopyExF(mRenderer, inTexture, nullptr, &dstRect, inRotation, nullptr, SDL_FLIP_NONE);

	// Reset SDL render color
	SDL_SetTextureColorMod(inTexture, 255, 255, 255);
	SDL_SetTextureAlphaMod(inTexture, 255);
}

SDL_FRect Renderer::GetSDLFRect(const fm::vec2& inPosition, const fm::vec2& inHalfSize)
{
	const fm::vec2 camera_position = mCamera->GetPosition();
	const float camera_zoom = mCamera->GetZoom();

	// Calculate the screen position of the center of the object
	fm::vec2 screen_center = (inPosition - camera_position) * camera_zoom;

	// Calculate the screen half size
	fm::vec2 screen_half_size = inHalfSize * camera_zoom;

	SDL_FRect sdl_frect;
	sdl_frect.x = screen_center.x - screen_half_size.x + static_cast<float>(mWindowSize.x) * 0.5f;
	sdl_frect.y = screen_center.y - screen_half_size.y + static_cast<float>(mWindowSize.y) * 0.5f;
	sdl_frect.w = screen_half_size.x * 2;
	sdl_frect.h = screen_half_size.y * 2;

	return sdl_frect;
}
