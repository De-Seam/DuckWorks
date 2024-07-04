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

class Renderer : public Manager
{
	RTTI_CLASS(Renderer, Manager, StandardAllocator)
public:
	struct ConstructParameters : public Base::ConstructParameters {};

	Renderer(const ConstructParameters& inConstructParameters = {}) : Base(inConstructParameters) {}

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

	virtual void Init() override;
	virtual void Shutdown() override;
	void BeginFrame();
	void EndFrame();
	virtual void Update(float inDeltaTime) override;

	struct DrawTextureParams
	{
		SDL_Texture* mTexture = nullptr;;
		fm::vec2 mPosition = {0, 0};
		fm::vec2 mHalfSize = {10000, 10000};
		float mRotation = 0.f;
		SDL_RendererFlip mFlip = SDL_FLIP_NONE;
		Optional<fm::ivec4> mSrcRect = NullOpt;
	};

	void DrawTexture(const DrawTextureParams& inParams);
	void DrawTextures(const Array<DrawTextureParams>& inParams);
	[[deprecated]] void DrawTextureTinted(const DrawTextureParams& inParams, const fm::vec4& inColor);

	struct DrawRectangleParams
	{
		fm::vec2 mPosition;
		fm::vec2 mHalfSize;
		fm::vec4 mColor;
	};

	void DrawRectangle(const DrawRectangleParams& inParams);

	void SetCamera(const SharedPtr<Camera>& inCamera) { mCamera = inCamera; }

	fm::vec2 GetWorldLocationAtWindowLocation(const fm::vec2& inWindowLocation) const;
	SDL_FRect GetSDLFRect(const fm::vec2& inPosition, const fm::vec2& inHalfSize);

	SDL_Window* GetWindow() const { return mWindow; }
	SDL_Renderer* GetRenderer() const { return mRenderer; }
	const fm::ivec2& GetWindowSize() const { return mWindowSize; }
	const SharedPtr<Camera>& GetCamera() const { return mCamera; }

	class RenderThreadTask : public ThreadTask
	{
	public:
		virtual void Execute() override;

		Array<DrawTextureParams> mCurrentDrawTextures;
		Array<DrawRectangleParams> mCurrentDrawRectangles;
	};

	const SharedPtr<RenderThreadTask>& GetRenderThreadTask() const { return mRenderThreadTask; }

private:
	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;

	// It's entirely possible for the camera not to have an entity. In that case, it's just a stationary camera
	SharedPtr<Camera> mCamera = nullptr;

	fm::ivec2 mWindowSize;

	Array<DrawTextureParams> mDrawTextures;
	Array<DrawRectangleParams> mDrawRectangles;

	SharedPtr<RenderThreadTask> mRenderThreadTask;

private:
	void UpdateCamera(float inDeltaTime);

	void DrawTextureInternal(const DrawTextureParams& inParams); ///< Doesn't lock the mutex
};

extern Renderer gRenderer;
