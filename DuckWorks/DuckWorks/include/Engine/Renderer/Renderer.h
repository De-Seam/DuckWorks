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
struct AABB;

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
		IVec2 mWindowSize;
		uint32 mWindowFlags;
		uint32 mRendererFlags;
	};

	virtual void Init() override;
	virtual void Shutdown() override;
	void BeginFrame();
	void EndFrame();
	virtual void Update(float inDeltaTime) override;

	enum class EDrawLayer : uint8
	{
		Background,
		Foreground,
		Effects,

		Count
	};

	struct DrawTextureParams
	{
		SDL_Texture* mTexture = nullptr;
		Vec2 mPosition = {0, 0};
		Vec2 mHalfSize = {10000, 10000};
		float mRotation = 0.f;
		Optional<IVec4> mSrcRect = NullOpt;
		SDL_RendererFlip mFlip = SDL_FLIP_NONE;
		EDrawLayer mLayer = EDrawLayer::Foreground;
	};

	void DrawTexture(const DrawTextureParams& inParams);
	void DrawTextures(const Array<DrawTextureParams>& inParams);

	struct DrawTextureTintedParams
	{
		DrawTextureParams mDrawTextureParams;
		Vec4 mColor;
	};

	void DrawTextureTinted(const DrawTextureTintedParams& inParams);

	struct DrawFilledRectangleParams
	{
		Transform2D mTransform;
		Vec4 mColor;
		EDrawLayer mLayer = EDrawLayer::Foreground;
	};

	void DrawFilledRectangle(const DrawFilledRectangleParams& inParams);

	struct DrawRectangleParams
	{
		Vec2 mPosition;
		Vec2 mHalfSize;
		Vec4 mColor;
		EDrawLayer mLayer = EDrawLayer::Foreground;
	};

	void DrawRectangle(const DrawRectangleParams& inParams);

	void DrawAABB(const AABB& inAABB, const Vec4& inColor);

	void SetCamera(const SharedPtr<Camera>& inCamera) { mCamera = inCamera; }
	void OverrideCameraThisFrame(const SharedPtr<Camera>& inCamera) { mOverrideCameraThisFrame = inCamera; }

	Vec2 GetWorldLocationAtWindowLocation(const Vec2& inWindowLocation) const;
	Vec2 GetRenderLocationAtWindowLocation(const Vec2& inWindowLocation) const;
	SDL_FRect GetSDLFRect(const Vec2& inPosition, const Vec2& inHalfSize, const SharedPtr<Camera>& inCamera);

	SDL_Window* GetWindow() const { return mWindow; }
	SDL_Renderer* GetRenderer() const { return mRenderer; }
	const IVec2& GetWindowSize() const { return mWindowSize; }
	const SharedPtr<Camera>& GetCamera() const { return mCamera; }

	class RenderThreadTask : public ThreadTask
	{
	public:
		virtual void Execute() override;

		StaticArray<Array<DrawTextureTintedParams>, SCast<uint8>(EDrawLayer::Count)> mCurrentDrawTexturesTinted;
		StaticArray<Array<DrawTextureParams>, SCast<uint8>(EDrawLayer::Count)> mCurrentDrawTextures;
		StaticArray<Array<DrawRectangleParams>, SCast<uint8>(EDrawLayer::Count)> mCurrentDrawRectangles;

		SharedPtr<Camera> mCamera = nullptr;
	};

	const SharedPtr<RenderThreadTask>& GetRenderThreadTask() const { return mRenderThreadTask; }

	bool IsDestRectOnScreen(const SDL_FRect& inDestRect) const;

private:
	void UpdateCamera(float inDeltaTime);

	void DrawTextureInternal(const DrawTextureParams& inParams); ///< Doesn't lock the mutex

	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;

	SDL_Texture* mWhiteTexture = nullptr;

	// It's entirely possible for the camera not to have an entity. In that case, it's just a stationary camera
	SharedPtr<Camera> mCamera = nullptr;
	SharedPtr<Camera> mOverrideCameraThisFrame = nullptr;

	IVec2 mWindowSize;

	StaticArray<Array<DrawTextureTintedParams>, SCast<uint8>(EDrawLayer::Count)> mDrawTexturesTinted;
	StaticArray<Array<DrawTextureParams>, SCast<uint8>(EDrawLayer::Count)> mDrawTextures;
	StaticArray<Array<DrawRectangleParams>, SCast<uint8>(EDrawLayer::Count)> mDrawRectangles;

	SharedPtr<RenderThreadTask> mRenderThreadTask;
};

extern Renderer gRenderer;
