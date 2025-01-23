#include "Engine.h"

// Engine includes
#include <App/App.h>

#include <Engine/Manager.h>
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

void Engine::sRegisterRTTI() 
{
	REGISTER_RTTI(Object);
	REGISTER_RTTI(Manager);
	REGISTER_RTTI(SDLEventManager);
	REGISTER_RTTI(FileManager);
	REGISTER_RTTI(ObjectManager);
	REGISTER_RTTI(Renderer);
	REGISTER_RTTI(TextureResource);
	REGISTER_RTTI(Resource);
	REGISTER_RTTI(ResourceManager);
}

Engine::Engine()
{
	gAssert(gEngine == nullptr);
	gEngine = this;

	FileManager& file_manager = TryCreateManager<FileManager>();
	file_manager.RegisterFileExtension<JsonFile>("json");

	mRenderer = &TryCreateManager<Renderer>();
	TryCreateManager<SDLEventManager>();
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

	mIsInitialized = true;
}

void Engine::Shutdown()
{
	mManagers.ForEach([](const DC::RTTITypeID&, Manager* inManager)
	{
		inManager->Shutdown();
	});

	mIsInitialized = false;
}

void Engine::BeginFrame()
{
	GetManager<Renderer>().BeginFrame();
}

void Engine::Update(float inDeltaTime)
{
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

void Engine::UnregisterUpdateCallback(const EngineUpdateHandle& inHandle)
{
	const int index = mUpdateCallbacks.FindIf([&inHandle](const UpdateCallback& inEntry){ return inEntry.mID == inHandle.GetID(); });
	gAssert(index != -1, "Handle not found");
	mUpdateCallbacks.Remove(index);
}
