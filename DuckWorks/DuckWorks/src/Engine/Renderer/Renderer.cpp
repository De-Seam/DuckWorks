#include "Precomp.h"
#include "Engine/Renderer/Renderer.h"

// Engine includes
#include "Engine/Debug/DebugUIWindowManager.h"
#include "Engine/Events/SDLEventManager.h"

// Game includes
#include "Game/App/App.h"

// External includes
#include "Engine/World/World.h"

#include "External/imgui/imgui.h"
#include "External/SDL/SDL.h"

Renderer gRenderer;

Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::Init(const InitParams& inInitParams)
{
	PROFILE_SCOPE(Renderer::Init)
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

	mCamera = std::make_shared<Camera>();

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

	gDebugUIWindowManager.Init();
}

void Renderer::Shutdown()
{
	PROFILE_SCOPE(Renderer::Shutdown)
	gLog(LogType::Info, "Shutting down Renderer");

	gDebugUIWindowManager.Shutdown();

	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
}

void Renderer::BeginFrame()
{
	PROFILE_SCOPE(Renderer::BeginFrame)

	// We handle ImGui new frame first so that it doesn't use the camera's render target
	gDebugUIWindowManager.BeginFrame();

	SDL_SetRenderTarget(mRenderer, mCamera->GetRenderTexture());

	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);
}

void Renderer::EndFrame()
{
	PROFILE_SCOPE(Renderer::EndFrame);

	SDL_SetRenderTarget(mRenderer, nullptr);
	SDL_RenderClear(mRenderer);

	SDL_Rect src_rect = {0, 0, static_cast<int32>(mCamera->GetSize().x), static_cast<int32>(mCamera->GetSize().y)};
	SDL_Rect dst_rect = {0, 0, mWindowSize.x, mWindowSize.y};
	SDL_RenderCopyEx(mRenderer, mCamera->GetRenderTexture(), &src_rect, &dst_rect, 0.0, nullptr, SDL_FLIP_NONE);

	gDebugUIWindowManager.EndFrame();

	SDL_RenderPresent(mRenderer);
}

void Renderer::Update(float inDeltaTime)
{
	PROFILE_SCOPE(Renderer::Update)

	UpdateCamera(inDeltaTime);
}

void Renderer::DrawTexture(const DrawTextureParams& inParams)
{
	const SDL_FRect dst_rect = GetSDLFRect(inParams.mPosition, inParams.mHalfSize);
	const SDL_Rect* src_rect = reinterpret_cast<const SDL_Rect*>(inParams.mSrcRect);
	SDL_RenderCopyExF(mRenderer, inParams.mTexture, src_rect, &dst_rect, inParams.mRotation, nullptr, inParams.mFlip);
}

void Renderer::DrawRectangle(const SDL_FRect& inRect, const fm::vec4& inColor)
{
	uint32 rgba = inColor.get_rgba();
	uint8 r = (rgba >> 24) & 0xFF;
	uint8 g = (rgba >> 16) & 0xFF;
	uint8 b = (rgba >> 8) & 0xFF;
	uint8 a = (rgba >> 0) & 0xFF;
	SDL_SetRenderDrawColor(mRenderer, r, g, b, a);
	SDL_RenderDrawRectF(mRenderer, &inRect);
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

fm::vec2 Renderer::GetWorldLocationAtWindowLocation(const fm::vec2& inWindowLocation) const
{
	const fm::vec2 camera_position = mCamera->GetPosition();
	const float camera_zoom = mCamera->GetZoom();
	const fm::vec2 camera_size = mCamera->GetSize(); // The render texture size (not used in this calculation)
	const fm::vec2 window_size = mWindowSize; // The actual window size

	fm::vec2 translated_position = inWindowLocation / (window_size / camera_size);

	// Step 1: Convert window coordinates to center-based coordinates relative to the window size
	fm::vec2 center_based_coords;
	center_based_coords.x = (translated_position.x - camera_size.x * 0.5f);
	center_based_coords.y = (translated_position.y - camera_size.y * 0.5f);

	// Step 2: Adjust for camera zoom
	// Since the zoom affects how much of the world is visible in the viewport,
	// and assuming the camera size defines the viewport size at zoom level 1,
	// we need to scale these coordinates by the inverse of the zoom to find their world space equivalent.
	fm::vec2 zoom_adjusted_coords = center_based_coords / camera_zoom;

	// Step 3: Adjust for camera position to get the world location
	fm::vec2 world_location = zoom_adjusted_coords + camera_position;

	return world_location;
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

void Renderer::UpdateCamera(float inDeltaTime)
{
	int32 highest_priority = INT32_MIN;
	SharedPtr<Camera> highest_priority_camera = {};
	entt::entity highest_priority_entity = entt::null;

	World* world = gApp.GetWorld();
	entt::registry& registry = world->GetRegistry();
	ScopedMutexReadLock lock(CameraComponent::sComponentMutex);
	auto view = registry.view<CameraComponent>();
	for (entt::entity entity : view)
	{
		const CameraComponent& camera_component = view.get<CameraComponent>(entity);
		if (camera_component.mIsActive)
		{
			if (camera_component.mPriority > highest_priority)
			{
				highest_priority = camera_component.mPriority;
				highest_priority_camera = camera_component.mCamera;
				highest_priority_entity = entity;
			}
		}
	}

	if (highest_priority_camera != nullptr)
	{
		mCamera = highest_priority_camera;

		BaseEntity base_entity = BaseEntity(highest_priority_entity, world);
		if (base_entity.HasComponent<TransformComponent>())
		{
			MutexReadProtectedPtr<TransformComponent> transform_component = base_entity.GetComponent<TransformComponent>();
			mCamera->SetPosition(transform_component->mTransform.position);
		}
	}

	mCamera->Update(inDeltaTime);
}
