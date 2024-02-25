#include "Precomp.h"
#include "Engine/Renderer/Renderer.h"

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
}

void Renderer::BeginFrame()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);
}

void Renderer::EndFrame()
{
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

SDL_FRect Renderer::GetSDLFRect(const fm::vec2& position, const fm::vec2& halfSize)
{
	const fm::vec2 cameraPosition = mCamera->GetPosition();
	const float cameraZoom = mCamera->GetZoom();

	// Calculate the screen position of the center of the object
	fm::vec2 screenCenter = (position - cameraPosition) * cameraZoom;

	// Calculate the screen half size
	fm::vec2 screenHalfSize = halfSize * cameraZoom;

	SDL_FRect sdlFRect;
	sdlFRect.x = screenCenter.x - screenHalfSize.x + static_cast<float>(mWindowSize.x) * 0.5f;
	sdlFRect.y = screenCenter.y - screenHalfSize.y + static_cast<float>(mWindowSize.y) * 0.5f;
	sdlFRect.w = screenHalfSize.x * 2;
	sdlFRect.h = screenHalfSize.y * 2;

	return sdlFRect;
}
