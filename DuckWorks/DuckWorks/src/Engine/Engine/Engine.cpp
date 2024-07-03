#include "Precomp.h"
#include "Engine/Engine/Engine.h"

// Engine includes
#include "Engine/Debug/DebugUIWindowManager.h"
#include "Engine/Engine/BaseUserSettings.h"
#include "Engine/Engine/FactoryRegistryEngine.h"
#include "Engine/Entity/Entity.h"
#include "Engine/Events/EventManager.h"
#include "Engine/Events/SDLEventManager.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Resources/ResourceManager.h"
#include "Engine/Threads/ThreadManager.h"
#include "Engine/Timer/TimerManager.h"
#include "Engine/World/World.h"

Engine gEngine;

void Engine::Init(UniquePtr<BaseUserSettings> inUserSettings)
{
	PROFILE_SCOPE(Engine::Init)
	gLog(ELogType::Info, "Initializing Engine");

	mUserSettings = std::move(inUserSettings);

	gLogManager.Init();

	gRegisterFactoryClassesEngine();

	gThreadManager.Init();

	{
		// Initialize Renderer
		Renderer::InitParams params;
		params.mWindowTitle = "DuckWorks";
		params.mWindowSize = GetUserSettings()->mWindowSize;
		params.mWindowFlags = GetUserSettings()->mWindowFlags;
		params.mRendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;
		gRenderer.Init(params);
	}

	{
		SDLEventFunction event_function;
		event_function.mEventType = SDL_QUIT;
		event_function.mFunctionPtr = [this](const SDL_Event&) { gEngine.Shutdown(); };
		gSDLEventManager.AddPersistentEventFunction(event_function);
	}

	{
		SDLEventFunction event_function;
		event_function.mEventType = SDL_WINDOWEVENT;
		event_function.mFunctionPtr = [this](const SDL_Event& inEvent)
		{
			if (inEvent.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				mUserSettings->mWindowSize.x = inEvent.window.data1;
				mUserSettings->mWindowSize.y = inEvent.window.data2;
			}
			else if (inEvent.window.event == SDL_WINDOWEVENT_MAXIMIZED)
			{
				mUserSettings->mWindowFlags |= SDL_WINDOW_MAXIMIZED;
			}
			else if (inEvent.window.event == SDL_WINDOWEVENT_RESTORED)
			{
				mUserSettings->mWindowFlags &= ~SDL_WINDOW_MAXIMIZED;
			}
		};
		gSDLEventManager.AddPersistentEventFunction(event_function);
	}

	gEventManager.Init();

	gResourceManager.Init();

	mLua.open_libraries(sol::lib::base);

	mLua.new_usertype<fm::vec2>("vec2",
								sol::constructors<fm::vec2(), fm::vec2(float, float)>(),
								"x", &fm::vec2::x,
								"y", &fm::vec2::y);

	mLua.new_usertype<fm::Transform>("Transform",
									sol::constructors<fm::Transform(), fm::Transform(fm::vec2, fm::vec2, float)>(),
									"position", &fm::Transform::position,
									"halfSize", &fm::Transform::halfSize,
									"rotation", &fm::Transform::rotation);

	mLua.new_usertype<Entity>("Entity",
							"GetTransform", &Entity::GetTransform,
							"SetTransform", &Entity::SetTransform);

	mLua["print"] = [](const String& inMessage)
	{
		gLog(ELogType::Info, inMessage.c_str());
	};
}

void Engine::Update(float inDeltaTime)
{
	mDeltaTime = inDeltaTime;

	gSDLEventManager.Update();

	gResourceManager.Update();

	if (!mIsPaused)
	{
		gTimerManager.Update(inDeltaTime);
		mWorld->Update(inDeltaTime);
	}
	mWorld->Render(inDeltaTime);

	{
		PROFILE_SCOPE(App::Update::WaitForRenderTask)
		// We wait until the render thread task is completed before beginning the next frame
		const SharedPtr<Renderer::RenderThreadTask>& render_thread_task = gRenderer.GetRenderThreadTask();
		if (render_thread_task != nullptr)
			render_thread_task->WaitUntilCompleted();
	}

	gDebugUIWindowManager.BeginFrame();
	gRenderer.BeginFrame();

	gDebugUIWindowManager.Update(inDeltaTime);
	gRenderer.Update(inDeltaTime);
}

bool Engine::FileExists(const char* inFilePath) const
{
	struct stat buffer;
	return (stat(inFilePath, &buffer) == 0);
}

void Engine::CreateNewWorld(const Json& inJson)
{
	PROFILE_SCOPE(App::CreateNewWorld)
	gLog(ELogType::Info, "Creating New World");

	if (mWorld != nullptr && mWorld->HasBegunPlay())
		mWorld->EndPlay();

	mWorld = nullptr;
	mWorld = std::make_unique<World>();

	if (!inJson.empty())
		mWorld->Deserialize(inJson);
}

void Engine::Deinitialize()
{
	PROFILE_SCOPE(Engine::Deinitialize)
	gLog(ELogType::Info, "Deinitializing Engine");

	mLua.collect_garbage();

	mWorld->EndPlay();
	mWorld = nullptr;

	gRenderer.Shutdown();

	gThreadManager.Shutdown();
	gLogManager.Shutdown();

	gEngine.Shutdown();
}
