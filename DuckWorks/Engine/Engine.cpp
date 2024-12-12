#include "Engine.h"

// Engine includes
#include <App/App.h>

#include <Engine/Manager.h>
#include <Engine/Events/SDLEventManager.h>
#include <Engine/Objects/ObjectManager.h>
#include <Engine/Renderer/Renderer.h>
#include <Engine/Resources/ResourceManager.h>

using namespace DC;

Engine* gEngine = nullptr;

EngineUpdateHandle::~EngineUpdateHandle()
{
	gAssert(gEngine != nullptr);
	gEngine->UnregisterUpdateCallback(*this);
}

Engine::Engine()
{
	gAssert(gEngine == nullptr);
	gEngine = this;
}

Engine::~Engine()
{
	gAssert(gEngine == this);

	if (mApp != nullptr)
	{
		mApp->Shutdown();
		mApp.Delete();
	}

	mManagers.Clear();

	gEngine = nullptr;
}

void Engine::Init()
{
	mManagers.ForEach([](const DC::RTTITypeID&, Manager* inManager)
	{
		inManager->Init();
	});
}

void Engine::Shutdown()
{
	mManagers.ForEach([](const DC::RTTITypeID&, Manager* inManager)
	{
		inManager->Shutdown();
	});
}

void Engine::BeginFrame()
{
	GetManager<Renderer>().BeginFrame();
}

void Engine::Update(float inDeltaTime)
{
	mApp->Update(inDeltaTime);

	for (const UpdateCallback& callback : mUpdateCallbacks)
		callback.mCallback(inDeltaTime);
}

void Engine::EndFrame()
{
	GetManager<Renderer>().EndFrame();
}

DC::Ref<EngineUpdateHandle> Engine::RegisterUpdateCallback(std::function<void(float)> inCallback)
{
	int next_id = mUpdateCallbacks.IsEmpty() ? 0 : mUpdateCallbacks.Back().mID + 1;
	gAssert(next_id >= 0, "IDs ran out");

	DC::Ref<EngineUpdateHandle> handle = new EngineUpdateHandle(next_id);
	mUpdateCallbacks.Emplace(handle->GetID(), inCallback);

	return handle;
}

void Engine::SetApp(DC::UniquePtr<App> inApp)
{
	if (mApp != nullptr)
		mApp->Shutdown();

	mApp = gMove(inApp);

	mApp->Init();
}

void Engine::RegisterApp(const String& inName, std::function<DC::UniquePtr<App>()> inConstructFunction)
{
	gAssert(!mApps.Contains(inName));
	mApps[inName] = gMove(inConstructFunction);
}

void Engine::UnregisterUpdateCallback(const EngineUpdateHandle& inHandle)
{
	const int index = mUpdateCallbacks.FindIf([&inHandle](const UpdateCallback& inEntry){ return inEntry.mID == inHandle.GetID(); });
	gAssert(index != -1, "Handle not found");
	mUpdateCallbacks.Remove(index);
}
