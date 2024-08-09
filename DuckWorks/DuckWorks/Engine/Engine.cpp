#include <Precomp.h>
#include <Engine/Engine.h>

// Engine includes
#include <Engine/Debug/DebugManager.h>
#include <Engine/Renderer/Renderer.h>
#include <Engine/Renderer/WindowEvents/WindowEventManager.h>
#include <Engine/Resources/ResourceManager.h>
#include <Engine/Threads/ThreadManager.h>

THREADLOCAL Engine* gEngine = nullptr;

Engine::Engine() 
{
	gAssert(gEngine == nullptr && "Engine already exists");
	gEngine = this;
}

Engine::~Engine() 
{
	gAssert(gEngine == this && "Engine does not exist");
	gEngine = nullptr;
}

Json Engine::Serialize() const
{
	Json json = RTTIClass::Serialize();
	JSON_SAVE(json, mRenderer);

	for (Manager* manager : mManagers)
	{
		if (manager != nullptr)
			json["mManagers"].push_back(manager->Serialize());
	}

	if (mWorld != nullptr)
		json["mWorld"] = mWorld->Serialize();

	return json;
}

void Engine::Deserialize(const Json& inJson)
{
	RTTIClass::Deserialize(inJson);
	mRenderer.Deserialize(inJson["mRenderer"]);

	const Json& json_managers = inJson["mManagers"];
	for (int32 i = 0; i < static_cast<int32>(json_managers.size()) && i < static_cast<int32>(mManagers.size()); i++)
	{
		if (mManagers[i] != nullptr)
			mManagers[i]->Deserialize(json_managers[i]);
	}

	if (mWorld != nullptr && inJson.contains("mWorld"))
		mWorld->Deserialize(inJson["mWorld"]);

	mJson = inJson;
}

void Engine::Init()
{
	PROFILE_SCOPE(Engine::Init)

	RegisterManagers();

	for (Manager* manager : mManagers)
		if (manager != nullptr)
			manager->Init();

	GetManager<WindowEventManager>().RegisterMessageListener(this, &Engine::OnWindowClosed);

	mWorld = std::make_unique<World>();
}

void Engine::Shutdown()
{
	PROFILE_SCOPE(Engine::Shutdown)

	mWorld = nullptr;

	for (Manager* manager : mManagers)
		if (manager != nullptr)
			manager->Shutdown();

	GetManager<WindowEventManager>().UnregisterMessageListener(this, &Engine::OnWindowClosed);
}

void Engine::Update(float inDeltaTime)
{
	PROFILE_SCOPE(Engine::Update)

	mRenderer.BeginFrame();

	for (Manager* manager : mManagersToUpdate)
		manager->Update(inDeltaTime);

	mWorld->Update(inDeltaTime);
	mRenderer.Update(inDeltaTime);

	mWorld->Render();

	mRenderer.EndFrame();
}

void Engine::RegisterManager(Manager* inManager) 
{
	PROFILE_SCOPE(Engine::RegisterManager)

	int32 type_id = inManager->GetManagerTypeID();
	if (type_id >= mManagers.size())
		mManagers.resize(type_id + 1);
	mManagers[type_id] = inManager;
	if (inManager->GetManagerSettings().mWantsUpdate)
		mManagersToUpdate.push_back(inManager);
	
	int32 index = -1;
	for (int32 i = 0; i < mJson["mManagers"].size(); i++)
	{
		String class_name = mJson["mManagers"][i]["ClassName"];
		if (class_name == inManager->GetRTTI().GetClassName())
		{
			index = i;
			break;
		}
	}
	if (index != -1)
		inManager->Deserialize(mJson["mManagers"][index]);
}

void Engine::OnWindowClosed(const MsgWindowClosed&) 
{
	RequestShutdown();
}

void Engine::RegisterManagers()
{	
	PROFILE_SCOPE(Engine::RegisterManagers)
	RegisterManager(&mRenderer.GetWindowEventManager());
	CreateManager<DebugManager>();
	CreateManager<ThreadManager>();
	CreateManager<ResourceManager>();
}
