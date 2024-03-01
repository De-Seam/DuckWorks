#include "Precomp.h"
#include "Engine/Renderer/Renderer.h"

#include "Engine/Events/SDLEventManager.h"
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

	// Window resize event
	SDLEventFunction event_function;
	event_function.mEventType = SDL_WINDOWEVENT;
	event_function.mFunctionPtr = [this](const SDL_Event& inEvent)
	{
		if (inEvent.window.event == SDL_WINDOWEVENT_RESIZED)
		{
			mWindowSize.x = inEvent.window.data1;
			mWindowSize.y = inEvent.window.data2;
		}
	};
	gSDLEventManager.AddPersistentEventFunction(event_function);
}

void Renderer::Shutdown()
{
	gLog(LogType::Info, "Shutting down Renderer");

	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
}

void Renderer::BeginFrame()
{
	OPTICK_EVENT("BeginFrame");

	// We handle ImGui new frame first so that it doesn't use the camera's render target
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	SDL_SetRenderTarget(mRenderer, mCamera->GetRenderTexture());

	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);
}

void Renderer::EndFrame()
{
	OPTICK_EVENT("Renderer::EndFrame");

	SDL_SetRenderTarget(mRenderer, nullptr);
	SDL_RenderClear(mRenderer);

	SDL_Rect src_rect = {0, 0, static_cast<int32>(mCamera->GetSize().x), static_cast<int32>(mCamera->GetSize().y)};
	SDL_Rect dst_rect = {0, 0, mWindowSize.x, mWindowSize.y};
	SDL_RenderCopyEx(mRenderer, mCamera->GetRenderTexture(), &src_rect, &dst_rect, 0.0, nullptr, SDL_FLIP_NONE);

	ImGui::Render();
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

	SDL_RenderPresent(mRenderer);
}

void Renderer::Update(float inDeltaTime)
{
	OPTICK_EVENT("Renderer::Update")

	mCamera->Update(inDeltaTime);
}

void Renderer::DrawTexture(const DrawTextureParams& inParams)
{
	const SDL_FRect dstRect = GetSDLFRect(inParams.mPosition, inParams.mHalfSize);
	const SDL_Rect* srcRect = reinterpret_cast<const SDL_Rect*>(inParams.mSrcRect);
	SDL_RenderCopyExF(mRenderer, inParams.mTexture, srcRect, &dstRect, inParams.mRotation, nullptr, inParams.mFlip);
}

void Renderer::DrawTextureTinted(const DrawTextureParams& inParams, const fm::vec4& inColor)
{
	// Calculate color components
	const uint32 argb = inColor.get_argb();
	const uint8 a = (argb >> 24) & 0xFF;
	const uint8 r = (argb >> 16) & 0xFF;
	const uint8 g = (argb >> 8) & 0xFF;
	const uint8 b = (argb >> 0) & 0xFF;

	// Set SDL render color
	SDL_SetTextureColorMod(inParams.mTexture, r, g, b);
	SDL_SetTextureAlphaMod(inParams.mTexture, a);

	const SDL_FRect dstRect = GetSDLFRect(inParams.mPosition, inParams.mHalfSize);
	const SDL_Rect* srcRect = reinterpret_cast<const SDL_Rect*>(inParams.mSrcRect);
	SDL_RenderCopyExF(mRenderer, inParams.mTexture, srcRect, &dstRect, inParams.mRotation, nullptr, inParams.mFlip);

	// Reset SDL render color
	SDL_SetTextureColorMod(inParams.mTexture, 255, 255, 255);
	SDL_SetTextureAlphaMod(inParams.mTexture, 255);
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
	sdl_frect.x = screen_center.x - screen_half_size.x + static_cast<float>(mCamera->GetSize().x) * 0.5f;
	sdl_frect.y = screen_center.y - screen_half_size.y + static_cast<float>(mCamera->GetSize().y) * 0.5f;
	sdl_frect.w = screen_half_size.x * 2;
	sdl_frect.h = screen_half_size.y * 2;

	return sdl_frect;
}
