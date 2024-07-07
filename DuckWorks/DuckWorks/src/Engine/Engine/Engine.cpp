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

	REGISTER_MANAGER(gLogManager);
	REGISTER_MANAGER(gTimerManager);
	REGISTER_MANAGER(gThreadManager);
	REGISTER_MANAGER(gRenderer);
	REGISTER_MANAGER(gSDLEventManager);
	REGISTER_MANAGER(gEventManager);
	REGISTER_MANAGER(gResourceManager);
	REGISTER_MANAGER(gDebugUIWindowManager);

	INIT_MANAGER_AFTER(gTimerManager, gLogManager);

	INIT_MANAGER_AFTER(gThreadManager, gLogManager);

	INIT_MANAGER_AFTER(gRenderer, gLogManager);
	INIT_MANAGER_AFTER(gRenderer, gThreadManager);

	INIT_MANAGER_AFTER(gSDLEventManager, gLogManager);
	INIT_MANAGER_AFTER(gSDLEventManager, gRenderer);

	INIT_MANAGER_AFTER(gEventManager, gSDLEventManager);
	// Update EventManager before SDLEventManager to reset old mouse position before getting callbacks
	UPDATE_MANAGER_AFTER(gSDLEventManager, gEventManager);
	INIT_MANAGER_AFTER(gEventManager, gRenderer);

	INIT_MANAGER_AFTER(gResourceManager, gLogManager);
	INIT_MANAGER_AFTER(gResourceManager, gRenderer);

	INIT_MANAGER_AFTER(gDebugUIWindowManager, gLogManager);
	INIT_MANAGER_AFTER(gDebugUIWindowManager, gSDLEventManager);
	INIT_MANAGER_AFTER(gDebugUIWindowManager, gEventManager);
	INIT_MANAGER_AFTER(gDebugUIWindowManager, gResourceManager);
	INIT_MANAGER_AFTER(gDebugUIWindowManager, gRenderer);

	gRegisterFactoryClassesEngine();

	InitManagers();

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
				mUserSettings->mWindowSize.mX = inEvent.window.data1;
				mUserSettings->mWindowSize.mY = inEvent.window.data2;
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

	mLua.open_libraries(sol::lib::base);

	mLua.new_usertype<Vec2>("Vec2",
							sol::constructors<Vec2(), Vec2(float, float)>(),
							"mX", &Vec2::mX,
							"mY", &Vec2::mY);

	mLua.new_usertype<Transform2D>("Transform",
									sol::constructors<Transform2D(), Transform2D(Vec2, Vec2, float)>(),
									"mPosition", &Transform2D::mPosition,
									"mHalfSize", &Transform2D::mHalfSize,
									"mRotation", &Transform2D::mRotation);

	mLua.new_usertype<Entity>("Entity",
							"GetPosition", &Entity::GetPosition,
							"SetPosition", &Entity::SetPosition);

	mLua["print"] = [](const String& inMessage)
	{
		gLog(ELogType::Info, inMessage.c_str());
	};
}

void Engine::Update(float inDeltaTime)
{
	mDeltaTime = inDeltaTime;

	for (Manager* manager : mManagersToUpdate)
	{
		gAssert(manager->RequiresUpdate());
		if (manager->ShouldUpdateWhilePaused())
			manager->Update(inDeltaTime);
	}

	if (!mIsPaused)
		mWorld->Update(inDeltaTime);

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

	gEngine.Shutdown();

	mLua.collect_garbage();

	mWorld->EndPlay();
	mWorld = nullptr;

	ShutdownManagers();
}

void Engine::RegisterManager(Manager& inManager)
{
	gLog(ELogType::Info, "Registering Manager: %s", inManager.GetClassName());
	mManagers.push_back(&inManager);
	if (inManager.RequiresUpdate())
	{
		gLog(ELogType::Info, "Enabling Updates for Manager: %s", inManager.GetClassName());
		mManagersToUpdate.push_back(&inManager);
	}
}

void Engine::InitManagerBefore(Manager& inManager, Manager& inOtherManager)
{
	mManagersToInitBeforeMap[&inManager].push_back(&inOtherManager);
}

void Engine::InitManagerAfter(Manager& inManager, Manager& inOtherManager)
{
	mManagersToInitAfterMap[&inManager].push_back(&inOtherManager);
}

void Engine::UpdateManagerBefore(Manager& inManager, Manager& inOtherManager)
{
	mManagersToUpdateBeforeMap[&inManager].push_back(&inOtherManager);
}

void Engine::UpdateManagerAfter(Manager& inManager, Manager& inOtherManager)
{
	mManagersToUpdateAfterMap[&inManager].push_back(&inOtherManager);
}

void Engine::sOrganizeArray(Array<Manager*>& ioArray, const HashMap<Manager*, Array<Manager*>>& inMapBefore, const HashMap<Manager*, Array<Manager*>>& inMapAfter)
{
	Array<Manager*> organized_array;
	organized_array.reserve(ioArray.size());

	for (Manager* manager : ioArray)
	{
		if (inMapAfter.contains(manager))
		{
			for (Manager* other_manager : inMapAfter.at(manager))
			{
				auto iter = std::ranges::find(organized_array, other_manager);
				if (iter == organized_array.end())
				{
					organized_array.push_back(other_manager);
				}
				else
				{
					if (iter < std::ranges::find(organized_array, manager))
						gAssert(false, "Manager is in the wrong order");
				}
			}
		}

		if (std::ranges::find(organized_array, manager) == organized_array.end())
		{
			organized_array.push_back(manager);
		}

		if (inMapBefore.contains(manager))
		{
			for (Manager* other_manager : inMapBefore.at(manager))
			{
				auto iter = std::ranges::find(organized_array, other_manager);
				if (iter == organized_array.end())
				{
					organized_array.push_back(other_manager);
				}
				else
				{
					if (iter > std::ranges::find(organized_array, manager))
						gAssert(false, "Manager is in the wrong order");
				}
			}
		}
	}

	ioArray = organized_array;
}

void Engine::InitManagers()
{
	sOrganizeArray(mManagers, mManagersToInitBeforeMap, mManagersToInitAfterMap);
	sOrganizeArray(mManagersToUpdate, mManagersToUpdateBeforeMap, mManagersToUpdateAfterMap);

	for (Manager* manager : mManagers)
		manager->Init();
}

void Engine::ShutdownManagers()
{
	for (auto iter = mManagers.rbegin(); iter != mManagers.rend(); ++iter)
		(*iter)->Shutdown();
}
