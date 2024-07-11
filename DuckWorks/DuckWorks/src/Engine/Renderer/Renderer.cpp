#include "Precomp.h"
#include "Engine/Renderer/Renderer.h"

// Engine includes
#include "Engine/Debug/DebugUIWindowManager.h"
#include "Engine/Engine/Engine.h"
#include "Engine/Entity/Components.h"
#include "Engine/Entity/Components/EntityComponentManager.h"
#include "Engine/Events/SDLEventManager.h"
#include "Engine/World/World.h"

// External includes
#include "External/SDL/SDL.h"

RTTI_CLASS_DEFINITION(Renderer, StandardAllocator)

RTTI_EMPTY_SERIALIZE_DEFINITION(Renderer)

Renderer gRenderer;

void Renderer::Init()
{
	PROFILE_SCOPE(Renderer::Init)
	gLog(ELogType::Info, "Initializing Renderer");

	SDL_SetMainReady();
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		gLog(ELogType::Error, "Error initializing SDL: %s\n", SDL_GetError());
		exit(1);
	}

	if (std::atexit(SDL_Quit) != 0)
	{
		gLog(ELogType::Error, "Error assigning SDL_Quit to atexit.");
	}

	BaseUserSettings* user_settings = gEngine.GetUserSettings();
	mWindow = SDL_CreateWindow("DuckWorks", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, user_settings->mWindowSize.mX,
								user_settings->mWindowSize.mY, user_settings->mWindowFlags);
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	mWindowSize = user_settings->mWindowSize;

	mCamera = std::make_shared<Camera>();

	// Window resize event
	SDLEventFunction event_function;
	event_function.mEventType = SDL_WINDOWEVENT;
	event_function.mFunctionPtr = [this](const SDL_Event& inEvent)
	{
		if (inEvent.window.event == SDL_WINDOWEVENT_RESIZED)
		{
			mWindowSize.mX = inEvent.window.data1;
			mWindowSize.mY = inEvent.window.data2;
		}
	};
	gSDLEventManager.AddPersistentEventFunction(event_function);

	// Create white texture
	{
		SDL_Surface* surface = SDL_CreateRGBSurface(0, 1, 1, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

		// Fill the surface with white color
		Uint32 green = SDL_MapRGB(surface->format, 255, 255, 255);
		SDL_FillRect(surface, nullptr, green);

		// Create a texture from the surface
		mWhiteTexture = SDL_CreateTextureFromSurface(mRenderer, surface);
		SDL_FreeSurface(surface); // We can free the surface after creating the texture
	}
}

void Renderer::Shutdown()
{
	PROFILE_SCOPE(Renderer::Shutdown)
	gLog(ELogType::Info, "Shutting down Renderer");

	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
}

void Renderer::BeginFrame()
{
	PROFILE_SCOPE(Renderer::BeginFrame)

	SDL_SetRenderTarget(mRenderer, mCamera->GetRenderTexture());

	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);
}

void Renderer::EndFrame()
{
	PROFILE_SCOPE(Renderer::EndFrame)

	SDL_SetRenderTarget(mRenderer, nullptr);
	SDL_RenderClear(mRenderer);

	SDL_Rect src_rect = {0, 0, static_cast<int32>(mCamera->GetSize().mX), static_cast<int32>(mCamera->GetSize().mY)};
	SDL_Rect dst_rect = {0, 0, mWindowSize.mX, mWindowSize.mY};
	SDL_RenderCopyEx(mRenderer, mCamera->GetRenderTexture(), &src_rect, &dst_rect, 0.0, nullptr, SDL_FLIP_NONE);

	IF_DEBUG(gDebugUIWindowManager.EndFrame();)

	SDL_RenderPresent(mRenderer);
}

void Renderer::Update(float inDeltaTime)
{
	PROFILE_SCOPE(Renderer::Update)
	gAssert(gIsMainThread());

	if (mRenderThreadTask == nullptr)
		mRenderThreadTask = std::make_unique<RenderThreadTask>();
	else
		gAssert(mRenderThreadTask->IsCompleted(), "Render Thread Task was not yet completed!");

	for (Array<DrawTextureTintedParams>& draw_texture_tinted_params : mRenderThreadTask->mCurrentDrawTexturesTinted)
		draw_texture_tinted_params.clear();
	for (Array<DrawTextureParams>& draw_texture_params : mRenderThreadTask->mCurrentDrawTextures)
		draw_texture_params.clear();
	for (Array<DrawRectangleParams>& draw_rectangle_params : mRenderThreadTask->mCurrentDrawRectangles)
		draw_rectangle_params.clear();

	swap(mDrawTexturesTinted, mRenderThreadTask->mCurrentDrawTexturesTinted);
	swap(mDrawTextures, mRenderThreadTask->mCurrentDrawTextures);
	swap(mDrawRectangles, mRenderThreadTask->mCurrentDrawRectangles);

	UpdateCamera(inDeltaTime);

	mRenderThreadTask->mCamera = mCamera;

	gThreadManager.AddTask(mRenderThreadTask, ThreadPriority::VeryHigh);
}

void Renderer::DrawTexture(const DrawTextureParams& inParams)
{
	DrawTextureInternal(inParams);
}

void Renderer::DrawTextures(const Array<DrawTextureParams>& inParams)
{
	for (const DrawTextureParams& params : inParams)
		DrawTextureInternal(params);
}

void Renderer::DrawTextureTinted(const DrawTextureTintedParams& inParams)
{
	gAssert(gIsMainThread());

	mDrawTexturesTinted[SCast<uint8>(inParams.mDrawTextureParams.mLayer)].emplace_back(inParams);
}

void Renderer::DrawFilledRectangle(const DrawFilledRectangleParams& inParams)
{
	DrawTextureTintedParams params;
	params.mDrawTextureParams.mTexture = mWhiteTexture;
	params.mDrawTextureParams.mPosition = inParams.mTransform.mPosition;
	params.mDrawTextureParams.mHalfSize = inParams.mTransform.mHalfSize;
	params.mDrawTextureParams.mRotation = inParams.mTransform.mRotation;
	params.mDrawTextureParams.mLayer = inParams.mLayer;
	params.mColor = inParams.mColor;
	DrawTextureTinted(params);
}

void Renderer::DrawRectangle(const DrawRectangleParams& inParams)
{
	gAssert(gIsMainThread());
	mDrawRectangles[SCast<uint8>(inParams.mLayer)].emplace_back(inParams);
}

void Renderer::DrawAABB(const AABB& inAABB, const Vec4& inColor)
{
	DrawRectangleParams params;
	params.mHalfSize = (inAABB.mMax - inAABB.mMin) * 0.5f;
	params.mPosition = inAABB.mMin + params.mHalfSize;
	params.mColor = inColor;
	DrawRectangle(params);
}

Vec2 Renderer::GetWorldLocationAtWindowLocation(const Vec2& inWindowLocation) const
{
	const Vec2 camera_position = mCamera->GetPosition();
	const float camera_zoom = mCamera->GetZoom();
	const Vec2 camera_size = mCamera->GetSize(); // The render texture size (not used in this calculation)
	const Vec2 window_size = mWindowSize; // The actual window size

	Vec2 translated_position = inWindowLocation / (window_size / camera_size);

	// Step 1: Convert window coordinates to center-based coordinates relative to the window size
	Vec2 center_based_coords;
	center_based_coords.mX = (translated_position.mX - camera_size.mX * 0.5f);
	center_based_coords.mY = (translated_position.mY - camera_size.mY * 0.5f);

	// Step 2: Adjust for camera zoom
	// Since the zoom affects how much of the world is visible in the viewport,
	// and assuming the camera size defines the viewport size at zoom level 1,
	// we need to scale these coordinates by the inverse of the zoom to find their world space equivalent.
	Vec2 zoom_adjusted_coords = center_based_coords / camera_zoom;

	// Step 3: Adjust for camera mPosition to get the world location
	Vec2 world_location = zoom_adjusted_coords + camera_position;

	return world_location;
}

SDL_FRect Renderer::GetSDLFRect(const Vec2& inPosition, const Vec2& inHalfSize, const SharedPtr<Camera>& inCamera)
{
	const Vec2 camera_position = inCamera->GetPosition();
	const float camera_zoom = inCamera->GetZoom();

	// Calculate the screen mPosition of the center of the object
	Vec2 screen_center = (inPosition - camera_position) * camera_zoom;

	// Calculate the screen half size
	Vec2 screen_half_size = inHalfSize * camera_zoom;

	SDL_FRect sdl_frect;
	sdl_frect.x = screen_center.mX - screen_half_size.mX + mCamera->GetSize().mX * 0.5f;
	sdl_frect.y = screen_center.mY - screen_half_size.mY + mCamera->GetSize().mY * 0.5f;
	sdl_frect.w = screen_half_size.mX * 2;
	sdl_frect.h = screen_half_size.mY * 2;

	return sdl_frect;
}

bool Renderer::IsDestRectOnScreen(const SDL_FRect& inDestRect) const
{
	return inDestRect.x + inDestRect.w >= 0 && inDestRect.x <= mWindowSize.mX && inDestRect.y + inDestRect.h >= 0 && inDestRect.y <= mWindowSize.mY;
}

void Renderer::UpdateCamera(float inDeltaTime)
{
	int32 highest_priority = INT32_MIN;
	SharedPtr<Camera> highest_priority_camera = {};
	CameraComponent* highest_component_camera = nullptr;

	if (mOverrideCameraThisFrame != nullptr)
	{
		highest_priority_camera = mOverrideCameraThisFrame;
		mOverrideCameraThisFrame = nullptr;
	}
	else
	{
		gEntityComponentManager.LoopOverComponents<CameraComponent>([&](CameraComponent& inCameraComponent)
		{
			if (inCameraComponent.mIsActive)
			{
				if (inCameraComponent.mPriority > highest_priority)
				{
					highest_priority = inCameraComponent.mPriority;
					highest_priority_camera = inCameraComponent.mCamera;
					highest_component_camera = &inCameraComponent;
				}
			}
		});
	}

	if (highest_priority_camera != nullptr)
	{
		mCamera = highest_priority_camera;

		if (highest_component_camera != nullptr)
		{
			Entity* entity = highest_component_camera->GetEntity();
			mCamera->SetPosition(entity->GetPosition());
		}
	}

	mCamera->Update(inDeltaTime);
}

void Renderer::DrawTextureInternal(const DrawTextureParams& inParams)
{
	gAssert(gIsMainThread());
	mDrawTextures[SCast<uint8>(inParams.mLayer)].emplace_back(inParams);
}

void Renderer::RenderThreadTask::Execute()
{
	PROFILE_SCOPE(Renderer::RenderThreadTask::Execute)

	SDL_Renderer* renderer = gRenderer.GetRenderer();

	for (uint64 i = 0; i < SCast<uint64>(EDrawLayer::Count); i++)
	{
		// Tinted textures
		for (const DrawTextureTintedParams& draw_texture_tinted_params : mCurrentDrawTexturesTinted[i])
		{
			const SDL_FRect dst_rect = gRenderer.GetSDLFRect(draw_texture_tinted_params.mDrawTextureParams.mPosition, draw_texture_tinted_params.mDrawTextureParams.mHalfSize, mCamera);
			if (!gRenderer.IsDestRectOnScreen(dst_rect))
				continue;

			// Calculate color components
			const uint32 argb = draw_texture_tinted_params.mColor.GetARGB();
			const uint8 a = (argb >> 24) & 0xFF;
			const uint8 r = (argb >> 16) & 0xFF;
			const uint8 g = (argb >> 8) & 0xFF;
			const uint8 b = (argb >> 0) & 0xFF;

			// Set SDL render color
			SDL_SetTextureColorMod(draw_texture_tinted_params.mDrawTextureParams.mTexture, r, g, b);
			SDL_SetTextureAlphaMod(draw_texture_tinted_params.mDrawTextureParams.mTexture, a);

			const SDL_Rect* src_rect = draw_texture_tinted_params.mDrawTextureParams.mSrcRect.has_value() ? RCast<const SDL_Rect*>(&draw_texture_tinted_params.mDrawTextureParams.mSrcRect.value()) : nullptr;
			SDL_RenderCopyExF(renderer, draw_texture_tinted_params.mDrawTextureParams.mTexture, src_rect, &dst_rect, draw_texture_tinted_params.mDrawTextureParams.mRotation, nullptr, draw_texture_tinted_params.mDrawTextureParams.mFlip);

			// Reset SDL render color
			SDL_SetTextureColorMod(draw_texture_tinted_params.mDrawTextureParams.mTexture, 255, 255, 255);
			SDL_SetTextureAlphaMod(draw_texture_tinted_params.mDrawTextureParams.mTexture, 255);
		}

		// Textures
		for (const DrawTextureParams& draw_texture_params : mCurrentDrawTextures[i])
		{
			const SDL_FRect dst_rect = gRenderer.GetSDLFRect(draw_texture_params.mPosition, draw_texture_params.mHalfSize, mCamera);
			if (!gRenderer.IsDestRectOnScreen(dst_rect))
				continue;

			const SDL_Rect* src_rect = draw_texture_params.mSrcRect.has_value() ? RCast<const SDL_Rect*>(&draw_texture_params.mSrcRect.value()) : nullptr;
			SDL_RenderCopyExF(renderer, draw_texture_params.mTexture, src_rect, &dst_rect, draw_texture_params.mRotation, nullptr, draw_texture_params.mFlip);
		}

		// Rectangles
		for (const DrawRectangleParams& draw_rectangle_params : mCurrentDrawRectangles[i])
		{
			SDL_FRect dst_rect = gRenderer.GetSDLFRect(draw_rectangle_params.mPosition, draw_rectangle_params.mHalfSize, mCamera);
			if (!gRenderer.IsDestRectOnScreen(dst_rect))
				continue;

			uint32 rgba = draw_rectangle_params.mColor.GetRGBA();
			uint8 r = (rgba >> 24) & 0xFF;
			uint8 g = (rgba >> 16) & 0xFF;
			uint8 b = (rgba >> 8) & 0xFF;
			uint8 a = (rgba >> 0) & 0xFF;
			SDL_SetRenderDrawColor(renderer, r, g, b, a);
			SDL_RenderDrawRectF(renderer, &dst_rect);
		}
		// Reset draw color
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	}

	gRenderer.EndFrame();
}
