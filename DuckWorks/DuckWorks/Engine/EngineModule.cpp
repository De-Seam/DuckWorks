#include <Precomp.h>
#include <Engine/EngineModule.h>

// Core includes
#include <Core/RTTI/Factory/RTTIFactory.h>

// Engine includes
#include <Engine/Engine.h>
#include <Engine/Debug/DebugManager.h>
#include <Engine/Debug/Windows/DebugWindow.h>
#include <Engine/Debug/Windows/DebugWindowObjectInspector.h>
#include <Engine/Renderer/WindowEvents/WindowEventManager.h>
#include <Engine/Resources/ResourceManager.h>
#include <Engine/Resources/TextureResource.h>

EngineModule* gEngineModule = nullptr;

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
	PROFILE_SCOPE(EngineModule::RegisterRTTI)

	CoreModule::RegisterRTTI();

	mRTTIFactory.RegisterClass<Engine>();
	mRTTIFactory.RegisterClass<Renderer>();
	mRTTIFactory.RegisterClass<Manager>();
	mRTTIFactory.RegisterClass<WindowEventManager>();
	mRTTIFactory.RegisterClass<ResourceManager>();
	mRTTIFactory.RegisterClass<Resource>();
	mRTTIFactory.RegisterClass<TextureResource>();

	mRTTIFactory.RegisterClass<DebugManager>();
	mRTTIFactory.RegisterClass<DebugWindow>();
	mRTTIFactory.RegisterClass<DebugWindowObjectInspector>();
}