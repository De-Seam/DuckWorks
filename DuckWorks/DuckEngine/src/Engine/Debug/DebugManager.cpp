#include <Engine/Precomp.h>
#include <Engine/Debug/DebugManager.h>

DebugManager::DebugManager() 
{
	mManagerSettings.mWantsUpdate = true;
}

void DebugManager::Init()
{
	Manager::Init();

}

void DebugManager::Shutdown()
{
	Manager::Shutdown();

}

void DebugManager::Update(float inDeltaTime)
{
	Manager::Update(inDeltaTime);

}

