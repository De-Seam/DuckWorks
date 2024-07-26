#include <Engine/Precomp.h>
#include <Engine/EngineModule.h>

// Core includes
#include <Core/RTTI/Factory/RTTIFactory.h>

// Engine includes
#include <Engine/Engine.h>

THREADLOCAL EngineModule* gEngineModule = nullptr;

EngineModule::EngineModule()
{
	gAssert(gEngineModule == nullptr);
	gEngineModule = this;
}

EngineModule::~EngineModule()
{
	gAssert(gEngineModule != nullptr);
	gEngineModule = nullptr;
}

void EngineModule::RegisterRTTI()
{
	mRTTIFactory.RegisterClass<Engine>();
}
