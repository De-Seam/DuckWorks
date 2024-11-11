#include "Engine.h"

// Engine includes
#include <Engine/Manager.h>
#include <Engine/Events/SDLEventManager.h>
#include <Engine/Objects/ObjectManager.h>
#include <Engine/Renderer/Renderer.h>

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

	CreateManager<ObjectManager>();
	CreateManager<SDLEventManager>();
	CreateManager<Renderer>();
}

Engine::~Engine()
{
	gAssert(gEngine == this);

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

void Engine::Update(float inDeltaTime)
{
	for (const UpdateCallback& callback : mUpdateCallbacks)
		callback.mCallback(inDeltaTime);

}

DC::Ref<EngineUpdateHandle> Engine::RegisterUpdateCallback(std::function<void(float)> inCallback)
{
	int next_id = mUpdateCallbacks.IsEmpty() ? 0 : mUpdateCallbacks.Back().mID + 1;
	gAssert(next_id >= 0, "IDs ran out");

	DC::Ref<EngineUpdateHandle> handle = new EngineUpdateHandle(next_id);
	mUpdateCallbacks.Emplace(handle->GetID(), inCallback);

	return handle;
}

void Engine::UnregisterUpdateCallback(const EngineUpdateHandle& inHandle)
{
	const int index = mUpdateCallbacks.FindIf([&inHandle](const UpdateCallback& inEntry){ return inEntry.mID == inHandle.GetID(); });
	gAssert(index != -1, "Handle not found");
	mUpdateCallbacks.Remove(index);
}
