#pragma once
// Core includes
#include "Core/Utilities/Utilities.h"

// Engine includes
#include "Engine/Renderer/Camera.h"

//External includes
#include "External/SDL/SDL_rect.h"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

class Renderer
{
public:
	Renderer();
	~Renderer();

	// Delete copy constructor
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;

	struct InitParams
	{
		String mWindowTitle;
		fm::ivec2 mWindowSize;
		uint32 mWindowFlags;
		uint32 mRendererFlags;
	};

	void Init(const InitParams& inInitParams);
	void Shutdown();
	void BeginFrame();
	void EndFrame();
	void Update(float inDeltaTime);

	void DrawTexture(SDL_Texture* inTexture, const fm::vec2& inPosition, const fm::vec2& inHalfSize, float inRotation, const fm::ivec4* inSrcRect = nullptr);
	void DrawTextureTinted(SDL_Texture* inTexture, const fm::vec2& inPosition, const fm::vec2& inHalfSize, float inRotation, const fm::vec4& inColor);

	SDL_FRect GetSDLFRect(const fm::vec2& inPosition, const fm::vec2& inHalfSize);

	SDL_Window* GetWindow() const { return mWindow; }
	SDL_Renderer* GetRenderer() const { return mRenderer; }
	const fm::ivec2& GetWindowSize() const { return mWindowSize; }

private:
	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;
	SDL_Texture* mRenderTargetTexture = nullptr;

	std::unique_ptr<Camera> mCamera = nullptr;

	fm::ivec2 mWindowSize;
};

extern Renderer gRenderer;
