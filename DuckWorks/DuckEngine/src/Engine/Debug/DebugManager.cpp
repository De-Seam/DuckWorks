#include <Engine/Precomp.h>
#include <Engine/Debug/DebugManager.h>

DebugManager::DebugManager() 
{
	mManagerSettings.mWantsUpdate = true;
}

void DebugManager::Init()
{

}

void DebugManager::Shutdown()
{

}

void DebugManager::Update(float inDeltaTime)
{
	PROFILE_SCOPE(DebugManager::Update)

}

