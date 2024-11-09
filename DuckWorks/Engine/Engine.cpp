#include "Engine.h"

// Engine includes
#include <Engine/Manager.h>
#include <Engine/Renderer/Renderer.h>

Engine* gEngine = nullptr;

EngineUpdateHandle::~EngineUpdateHandle()
{
	gAssert(gEngine != nullptr);
	gEngine->UnregisterUpdateCallback(*this);
}

Engine::Engine()
{
	gAssert(gEngine == nullptr);

	mManagers.Add(DC::UniquePtr<Renderer>::sMakeUnique());
}

Engine::~Engine()
{
	gAssert(gEngine == this);

	mManagers.Clear();
}

void Engine::Init()
{
	for (Manager* manager : mManagers)
		manager->Init();
}

void Engine::Shutdown()
{
	for (Manager* manager : mManagers)
		manager->Shutdown();
}

void Engine::Update(float inDeltaTime)
{
	
}

DC::Ref<EngineUpdateHandle> Engine::RegisterUpdateCallback(std::function<void(float)> inCallback)
{
	int next_id = mUpdateCallbacks.IsEmpty() ? 0 : mUpdateCallbacks.Back().first + 1;
	gAssert(next_id >= 0, "IDs ran out");

	DC::Ref<EngineUpdateHandle> handle = new EngineUpdateHandle(next_id);
	mUpdateCallbacks.Emplace(handle->GetID(), inCallback);

	return handle;
}

void Engine::UnregisterUpdateCallback(const EngineUpdateHandle& inHandle)
{
	const int index = mUpdateCallbacks.FindIf([&inHandle](const std::pair<int, std::function<void(float)>>& inEntry){ return inEntry.first == inHandle.GetID(); });
	gAssert(index != -1, "Handle not found");
	mUpdateCallbacks.Remove(index);
}
