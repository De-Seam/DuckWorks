#include <Engine/Precomp.h>
#include <Engine/Debug/DebugManager.h>

// Engine includes
#include <Engine/EngineModule.h>
#include <Engine/Debug/Windows/DebugWindow.h>

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
	THREADLOCAL static Array<String> sDebugWindowNames;
	sDebugWindowNames.clear();
	gEngineModule->mRTTIFactory.GetSubClassNames<DebugWindow>(sDebugWindowNames);
	for (const String& window_name : sDebugWindowNames)
	{
	}
}

