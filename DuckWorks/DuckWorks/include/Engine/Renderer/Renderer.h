#pragma once
// Core includes
#include "Core/Utilities/Utilities.h"

// Engine includes
#include "Engine/Renderer/Camera.h"
#include "Engine/Threads/ThreadManager.h"

//External includes
#include "External/SDL/SDL_rect.h"
#include "External/SDL/SDL_render.h"

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

	struct DrawTextureParams
	{
		SDL_Texture* mTexture = nullptr;;
		fm::vec2 mPosition = {0, 0};
		fm::vec2 mHalfSize = {10000, 10000};
		float mRotation = 0.f;
		SDL_RendererFlip mFlip = SDL_FLIP_NONE;
		const fm::ivec4* mSrcRect = nullptr;
	};

	void DrawTexture(const DrawTextureParams& inParams);
	void DrawTextureTinted(const DrawTextureParams& inParams, const fm::vec4& inColor);
	void DrawRectangle(const SDL_FRect& inRect, const fm::vec4& inColor);

	void SetCamera(const SharedPtr<Camera>& inCamera) { mCamera = inCamera; }

	fm::vec2 GetWorldLocationAtWindowLocation(const fm::vec2& inWindowLocation) const;
	SDL_FRect GetSDLFRect(const fm::vec2& inPosition, const fm::vec2& inHalfSize);

	SDL_Window* GetWindow() const { return mWindow; }
	SDL_Renderer* GetRenderer() const { return mRenderer; }
	const fm::ivec2& GetWindowSize() const { return mWindowSize; }
	const SharedPtr<Camera>& GetCamera() const { return mCamera; }

	struct RenderTextureData
	{
		SDL_Texture* mTexture = nullptr;
		SDL_Rect mSourceRectangle;
		bool mUseSourceRectangle = false;
		SDL_FRect mDestinationRectangle;
		float mRotation;
		SDL_RendererFlip mFlip;
	};

	struct RenderRectangleData
	{
		SDL_FRect mRectangle;
		fm::vec4 mColor;
	};

	class RenderThreadTask : public ThreadTask
	{
	public:
		virtual void Execute();

		Array<Renderer::RenderTextureData> mCurrentRenderTextureDatas;
		Array<Renderer::RenderRectangleData> mCurrentRenderRectangleDatas;
	};

	const SharedPtr<RenderThreadTask>& GetRenderThreadTask() const { return mRenderThreadTask; }

private:
	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;

	// It's entirely possible for the camera not to have an entity. In that case, it's just a stationary camera
	SharedPtr<Camera> mCamera = nullptr;

	fm::ivec2 mWindowSize;

	Array<RenderTextureData> mRenderTextureDatas;
	UniqueMutex mRenderTextureDatasMutex;

	Array<RenderRectangleData> mRenderRectangleDatas;
	UniqueMutex mRenderRectangleDatasMutex;

	SharedPtr<RenderThreadTask> mRenderThreadTask;

private:
	void UpdateCamera(float inDeltaTime);
};

extern Renderer gRenderer;
