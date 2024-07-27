#include <Engine/Precomp.h>
#include <Engine/EngineModule.h>

// Core includes
#include <Core/RTTI/Factory/RTTIFactory.h>

// Engine includes
#include <Engine/Engine.h>
#include <Engine/Renderer/WindowEvents/WindowEventManager.h>
#include <Engine/Resources/ResourceManager.h>
#include <Engine/Resources/TextureResource.h>

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
	mRTTIFactory.RegisterClass<Renderer>();
	mRTTIFactory.RegisterClass<Manager>();
	mRTTIFactory.RegisterClass<WindowEventManager>();
	mRTTIFactory.RegisterClass<ResourceManager>();
	mRTTIFactory.RegisterClass<Resource>();
	mRTTIFactory.RegisterClass<TextureResource>();
}
