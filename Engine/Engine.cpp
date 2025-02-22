#include <Engine/Engine.h>

#include <DuckCore/Events/EventManager.h>
#include <DuckCore/Managers/Manager.h>
#include <DuckCore/Managers/Managers.h>

#include <Engine/Events/SDLEventManager.h>
#include <Engine/Files/FileManager.h>
#include <Engine/Objects/ObjectManager.h>
#include <Engine/Renderer/Renderer.h>
#include <Engine/Renderer/TextureResource.h>
#include <Engine/Resources/JsonFile.h>

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

	Managers::sAdd(new EventManager);
	Managers::sAdd(new FileManager);
	FileManager& file_manager = Managers::sGet<FileManager>();
	file_manager.RegisterFileExtension<JsonFile>("json");

	Managers::sAdd(new Renderer);
	Managers::sAdd(new SDLEventManager);
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
