#include <Engine/Engine.h>

#include <DuckCore/Events/EventManager.h>
#include <DuckCore/Manager/Manager.h>
#include <DuckCore/Manager/Managers.h>

#include <Engine/Events/SDLEventManager.h>
#include <Engine/Objects/ObjectManager.h>
#include <Engine/Renderer/Renderer.h>
#include <Engine/Renderer/TextureResource.h>
#include <Engine/Resources/JsonFile.h>
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

	FileManager* file_manager = new FileManager;
	file_manager->RegisterFileExtension<JsonFile>("json");
	Managers::sAdd(file_manager);

	Managers::sAdd(new Renderer);
	Managers::sAdd(new SDLEventManager);
	Managers::sAdd(new ResourceManager);
	Managers::sAdd(new EventManager);
}

Engine::~Engine()
{
	gAssert(gEngine == this);

	gEngine = nullptr;
}

void Engine::BeginFrame()
{
	Managers::sGet<Renderer>().BeginFrame();
}

void Engine::Update(float inDeltaTime)
{
	for (const UpdateCallback& callback : mUpdateCallbacks)
		callback.mCallback(inDeltaTime);
}

void Engine::EndFrame()
{
	Managers::sGet<Renderer>().EndFrame();
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
